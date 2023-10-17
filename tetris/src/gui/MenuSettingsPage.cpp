#include <gui/MenuSettingsPage.h>
#include <Input.h>
#include <Window.h>

#include <SDL2/SDL.h>

MenuSettingsPage::MenuSettingsPage(const Window& window) : m_Window{window}
{
    // m_MenuItems.push_back(MenuItem(*window.GetRendererHandle(), "Menu Item 1", [&](){ SDL_Log("Clicked Menu Item 1"); }));
    // m_MenuItems.push_back(MenuItem(*window.GetRendererHandle(), "Menu Item 2", [&](){ SDL_Log("Clicked Menu Item 2"); }));
}

auto MenuSettingsPage::OnRender() -> void
{
    for(auto&& menuItem: m_MenuItems)
    {
        menuItem.OnRender();
    }
}

auto MenuSettingsPage::OnInput() -> void
{
    // if (Input::IsKeyPressed(m_Window, SDLK_DOWN)) 
    // {
    //     m_MenuItems[m_CurrentPageItemIndex].SetState(false);

    //     m_CurrentPageItemIndex++;

    //     if (m_CurrentPageItemIndex >= m_MenuItems.size()) 
    //     {
    //         m_CurrentPageItemIndex = 0;
    //     }

    //     m_MenuItems[m_CurrentPageItemIndex].SetState(true);
    // }

    // if (Input::IsKeyPressed(m_Window, SDLK_UP)) 
    // {
    //     m_MenuItems[m_CurrentPageItemIndex].SetState(false);

    //     m_CurrentPageItemIndex--;

    //     if (m_CurrentPageItemIndex < 0) 
    //     {
    //         m_CurrentPageItemIndex = m_MenuItems.size() - 1;
    //     }

    //     m_MenuItems[m_CurrentPageItemIndex].SetState(true);
    // }
}