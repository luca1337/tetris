#include <Input.h>

std::unordered_map<SDL_Keycode, bool> Input::m_KeyStates = {};

auto Input::IsKeyPressed(const Window &window, SDL_Keycode key) -> bool
{
    auto currentKeyState = window.GetKeyState();

    auto pressed = false;

    if (currentKeyState[SDL_GetScancodeFromKey(key)] && !m_KeyStates[key])
    {
        pressed = true;
    }

    m_KeyStates[key] = currentKeyState[SDL_GetScancodeFromKey(key)];

    return pressed;
}
