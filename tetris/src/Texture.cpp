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

Texture::Texture(SDL_Renderer *sdlRenderer, const std::string &filePath, bool useBlending)
{
    int width, height, color_channel;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &color_channel, STBI_rgb_alpha);

    if (!data)
    {
        SDL_Log("Failed to load image: %s", stbi_failure_reason());
        return;
    }

    m_SdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);

    if (!m_SdlTexture)
    {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        stbi_image_free(data);
        return;
    }

    int pitch = {};
    unsigned char* pixel = {};

    if (SDL_LockTexture(m_SdlTexture, nullptr, reinterpret_cast<void**>(&pixel), &pitch))
    {
        SDL_Log("Unable to lock texture: %s", SDL_GetError());
        stbi_image_free(data);
        SDL_DestroyTexture(m_SdlTexture);
        return;
    }

    memset(pixel, 0, static_cast<size_t>(width) * height * 4);
    memcpy(pixel, data, static_cast<size_t>(width) * height * 4);

    if (useBlending)
    {
        SDL_SetTextureBlendMode(m_SdlTexture, SDL_BLENDMODE_BLEND);
    }

    SDL_UnlockTexture(m_SdlTexture);

    stbi_image_free(data);
}

Texture::Texture(SDL_Renderer* sdlRenderer, const unsigned width, const unsigned height, const Color color)
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
    m_Quad.w = static_cast<int>(width);
    m_Quad.h = static_cast<int>(height);
}

auto Texture::Draw(SDL_Renderer* sdlRenderer, const uint8_t alpha) const -> void
{
    if (alpha > 0)
    {
        SDL_SetTextureAlphaMod(m_SdlTexture, alpha);
    }

    SDL_RenderCopy(sdlRenderer, m_SdlTexture, nullptr, &m_Quad);
}

auto Texture::SetPositionOnScreen(int x, int y) -> void
{
    m_Quad.x = x;
    m_Quad.y = y;
}
