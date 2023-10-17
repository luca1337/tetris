#pragma once

#include <string>
#include <Color.h>

#include <SDL_ttf.h>

struct SDL_Surface;
struct SDL_Texture;
struct SDL_Renderer;

class RenderText
{
public:
    ~RenderText();
    RenderText(SDL_Renderer* renderer, const std::string& fontPath, const int fontSize = 16);
    auto SetText(const std::string& text) { m_Text = text; }
    auto SetColor(const Color color) { m_Color = color; }

    auto Update(const std::string& newText) -> void;
    auto Draw(float x, float y) -> void;

private:
    SDL_Renderer* m_Renderer = {};
    SDL_Surface* m_Surface = {};
    SDL_Texture* m_CopyTexture = {};
    TTF_Font* m_Font = {};

private:
    int m_Width = {};
    int m_Height = {};
    Color m_Color = {};
    std::string m_Text = {};
};