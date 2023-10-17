#pragma once

#include <gui/MenuItem.h>

class Window;
struct SDL_Renderer;

class MenuSettingsPage
{
public:
    MenuSettingsPage(const Window& window);
    auto OnRender() -> void;
    auto OnInput() -> void;
private:
    const Window& m_Window;
    std::vector<MenuItem> m_MenuItems = {};
    int m_CurrentPageItemIndex = {};
};