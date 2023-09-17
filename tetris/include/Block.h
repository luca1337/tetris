#pragma once

#include <Color.h>

#include <vector>
#include <memory>
#include <SDL2/SDL.h>

class Texture;

class Block
{
public:
    Block() = default;
    Block(SDL_Renderer* sdlRenderer, int x, int y, Color color);

    const auto& GetTexture() const { return m_Texture; }
    auto Draw(SDL_Renderer* sdlRenderer, uint8_t alpha) -> void;
    auto& Index() { return index; }

private:
    std::shared_ptr<Texture> m_Texture = {};
    int index = {};
};