#include <RenderText.h>

#include <SDL2/SDL.h>

RenderText::~RenderText()
{
    if (m_Surface) { SDL_FreeSurface(m_Surface); }
    if (m_CopyTexture) { SDL_DestroyTexture(m_CopyTexture); }
}

RenderText::RenderText(SDL_Renderer *renderer, const std::string &fontPath, const int fontSize) : m_Renderer{renderer}
{
    m_Font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!m_Font)
    {
        SDL_Log("Error on opening font: %s", TTF_GetError());
        return;
    }

    const auto textColor = SDL_Color{ m_Color.r, m_Color.g, m_Color.b, m_Color.a };
    m_Surface = TTF_RenderUTF8_Blended(m_Font, m_Text.c_str(), textColor);
    m_CopyTexture = SDL_CreateTextureFromSurface(renderer, m_Surface);
}

auto RenderText::Update(const std::string &newText) -> void
{
    SDL_FreeSurface(m_Surface);
    SDL_DestroyTexture(m_CopyTexture);

    m_Surface = {};
    m_CopyTexture = {};

    m_Surface = TTF_RenderUTF8_Blended(m_Font, newText.c_str(), SDL_Color{ m_Color.r, m_Color.g, m_Color.b, m_Color.a });
    m_CopyTexture = SDL_CreateTextureFromSurface(m_Renderer, m_Surface);

    // also update width and height that might be changed if the new text is wider or smaller
    SDL_QueryTexture(m_CopyTexture, nullptr, nullptr, &m_Width, &m_Height);
}

auto RenderText::Draw(float x, float y) -> void
{
    const auto dstRect = SDL_FRect{x - (m_Width / 2), y - (m_Height / 2), static_cast<float>(m_Width), static_cast<float>(m_Height)};
    SDL_RenderCopyF(m_Renderer, m_CopyTexture, nullptr, &dstRect);
}
