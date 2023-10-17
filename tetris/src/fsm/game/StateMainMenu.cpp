#include <fsm/game/StateMainMenu.h>
#include <Window.h>
#include <gui/MenuItem.h>
#include <Input.h>
#include <game/Game.h>

#include <SDL2/SDL.h>

StateMainMenu::StateMainMenu(const Window &window, Game& game) : FSM{window}
{
    const auto onePlayerMenuItem = std::make_shared<MenuItem>(*m_Window.GetRendererHandle(), glm::vec2{(window.Props().width / 2) - 100, window.Props().height / 2}, "> 1 PLAYER", [this](){
         auto& playState = GetState(StateType::Play); 
         playState->OnStateEnter();
         m_CurrentState = playState;
    });
    const auto twoPlayerMenuItem = std::make_shared<MenuItem>(*m_Window.GetRendererHandle(), glm::vec2{(window.Props().width / 2) + 100, window.Props().height / 2}, "> 2 PLAYERS", [](){ SDL_Log("2 Players clicked"); });
    const auto settingsMenuItem =  std::make_shared<MenuItem>(*m_Window.GetRendererHandle(), glm::vec2{(window.Props().width / 2), window.Props().height / 2 + 75}, "> SETTINGS", [](){ SDL_Log("Settings clicked"); });

    m_MenuItems.push_back(onePlayerMenuItem);
    m_MenuItems.push_back(twoPlayerMenuItem);
    m_MenuItems.push_back(settingsMenuItem);
}

auto StateMainMenu::OnStateEnter() -> void
{
    m_CurrentMenuItem = m_MenuItems.at(0);
    m_CurrentMenuItem->SetState(true);
}

auto StateMainMenu::OnStateUpdate(float deltaTime) -> std::shared_ptr<IState>
{
    if (Input::IsKeyPressed(m_Window, SDLK_DOWN)) 
    {
        m_MenuItems[m_CurrentPageItemIndex]->SetState(false);

        m_CurrentPageItemIndex++;

        if (m_CurrentPageItemIndex >= m_MenuItems.size()) 
        {
            m_CurrentPageItemIndex = 0;
        }

        m_MenuItems[m_CurrentPageItemIndex]->SetState(true);
        m_CurrentMenuItem = m_MenuItems[m_CurrentPageItemIndex];
    }

    if (Input::IsKeyPressed(m_Window, SDLK_UP)) 
    {
        m_MenuItems[m_CurrentPageItemIndex]->SetState(false);

        m_CurrentPageItemIndex--;

        if (m_CurrentPageItemIndex < 0) 
        {
            m_CurrentPageItemIndex = m_MenuItems.size() - 1;
        }

        m_MenuItems[m_CurrentPageItemIndex]->SetState(true);
        m_CurrentMenuItem = m_MenuItems[m_CurrentPageItemIndex];
    }

    if (Input::IsKeyPressed(m_Window, SDLK_SPACE))
    {
        m_CurrentMenuItem->OnSelect();
    }

    for (auto&& menuItem: m_MenuItems)
    {
        menuItem->OnRender();
    }

    if (!m_CurrentState) return shared_from_this();
    else return m_CurrentState;
}