#include <Texture.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::~Texture()
{
    if (m_SdlTexture)
    {
        SDL_DestroyTexture(m_SdlTexture);
    }
}

Texture::Texture(const Texture& other)
{
    m_Width = other.m_Width;
    m_Height = other.m_Height;
    m_Color = other.m_Color;
    m_SdlRenderer = other.m_SdlRenderer;
    m_IsLoadedFromFile = other.m_IsLoadedFromFile;
    m_UseBlending = other.m_UseBlending;

    if (!m_IsLoadedFromFile)
    {
        constexpr auto depth = 32;
        const auto surface = SDL_CreateRGBSurface(0, m_Width, m_Height, depth, 0, 0, 0, 0);

        if (!surface)
        {
            SDL_Log("Could not create surface: %s", SDL_GetError());
            return;
        }

        SDL_FillRect(surface, nullptr, SDL_MapRGBA(surface->format, m_Color.r, m_Color.g, m_Color.b, m_Color.a));

        m_SdlTexture = SDL_CreateTextureFromSurface(m_SdlRenderer, surface);
        SDL_SetTextureBlendMode(m_SdlTexture, SDL_BLENDMODE_BLEND);

        SDL_FreeSurface(surface);
    }
    else
    {
        // Crea la nuova texture
        m_SdlTexture = SDL_CreateTexture(m_SdlRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, m_Width, m_Height);
        if (!m_SdlTexture) 
        {
            SDL_Log("Failed to create texture: %s", SDL_GetError());
            return;
        }

        int pitch = 0;

        // Blocca la texture e ottieni il puntatore ai pixel
        if (SDL_LockTexture(m_SdlTexture, nullptr, reinterpret_cast<void**>(&m_Pixels), &pitch)) 
        {
            SDL_Log("Unable to lock texture: %s", SDL_GetError());
            SDL_DestroyTexture(m_SdlTexture);
            return;
        }

        // Copia i pixel dal buffer dell'immagine alla texture
        memcpy(m_Pixels, other.m_Pixels, static_cast<size_t>(m_Width) * m_Height * 4);

        if (m_UseBlending)
        {
            SDL_SetTextureBlendMode(m_SdlTexture, SDL_BLENDMODE_BLEND);
        }

        // Sblocca la texture
        SDL_UnlockTexture(m_SdlTexture);
    }

    m_Quad = other.m_Quad;
}

Texture::Texture(SDL_Renderer *sdlRenderer, const std::string &filePath, bool useBlending) : m_SdlRenderer{sdlRenderer}
{
    int width, height, color_channel;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &color_channel, STBI_rgb_alpha);

    if (!data)
    {
        SDL_Log("Failed to load image: %s", stbi_failure_reason());
        return;
    }

    m_SdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);

    if (!m_SdlTexture)
    {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        stbi_image_free(data);
        return;
    }

    int pitch = {};

    if (SDL_LockTexture(m_SdlTexture, nullptr, reinterpret_cast<void**>(&m_Pixels), &pitch))
    {
        SDL_Log("Unable to lock texture: %s", SDL_GetError());
        stbi_image_free(data);
        SDL_DestroyTexture(m_SdlTexture);
        return;
    }

    memset(m_Pixels, 0, static_cast<size_t>(width) * height * 4);
    memcpy(m_Pixels, data, static_cast<size_t>(width) * height * 4);

    if (useBlending)
    {
        SDL_SetTextureBlendMode(m_SdlTexture, SDL_BLENDMODE_BLEND);
    }

    SDL_UnlockTexture(m_SdlTexture);

    stbi_image_free(data);

    m_UseBlending = useBlending;
    m_IsLoadedFromFile = true;
    m_Width = width;
    m_Height = height;
    m_Quad.w = width;
    m_Quad.h = height;
    m_Color = {};
}

Texture::Texture(SDL_Renderer* sdlRenderer, const unsigned width, const unsigned height, const Color color) : m_SdlRenderer{sdlRenderer}
{
    m_SdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, static_cast<int>(width), static_cast<int>(height));

    if (!m_SdlTexture)
    {
        SDL_Log("Could not create texture: %s", SDL_GetError());
        return;
    }

    int pitch = 0;
    unsigned char* pixels = nullptr;

    if (SDL_LockTexture(m_SdlTexture, nullptr, reinterpret_cast<void**>(&pixels), &pitch))
    {
        SDL_Log("Unable to lock texture: %s", SDL_GetError());
        SDL_DestroyTexture(m_SdlTexture);
        return;
    }

    const unsigned char colors[4] = { color.r, color.g, color.b, color.a };

    for (auto y = 0ul; y != height; ++y)
    {
        for (auto x = 0ul; x != width; ++x)
        {
            memcpy(&pixels[(static_cast<size_t>(y) * width + static_cast<size_t>(x)) * 4], colors, sizeof colors);
        }
    }

    SDL_SetTextureBlendMode(m_SdlTexture, SDL_BLENDMODE_BLEND);
    SDL_UnlockTexture(m_SdlTexture);

    m_Width = width;
    m_Height = height;
    m_Quad.w = width;
    m_Quad.h = height;
    m_Color = { color.r, color.g, color.b, color.a };
}

auto Texture::Draw(SDL_Renderer* sdlRenderer, const uint8_t alpha) const -> void
{
    if (alpha > 0)
    {
        SDL_SetTextureAlphaMod(m_SdlTexture, alpha);
    }

    SDL_RenderCopyF(sdlRenderer, m_SdlTexture, nullptr, &m_Quad);
}

auto Texture::SetPositionOnScreen(int x, int y) -> void
{
    m_Quad.x = x;
    m_Quad.y = y;
}
