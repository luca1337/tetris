#pragma once

#include <unordered_map>
#include <Window.h>
#include <SDL2/SDL.h>

class Input
{
public:
    [[nodiscard]] static auto IsKeyPressed(const Window& window, SDL_Keycode key) -> bool;

private:
    static std::unordered_map<SDL_Keycode, bool> m_KeyStates;
};