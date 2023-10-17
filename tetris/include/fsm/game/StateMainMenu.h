#pragma once

#include <fsm/base/FSM.h>
#include <Board.h>

class RenderText;
class Window;
class MenuItem;
class Game;
class IState;

class StateMainMenu : public FSM
{
public:
    ~StateMainMenu() = default;
    StateMainMenu(const Window& window, Game& game);

    auto OnStateEnter() -> void override;
    auto OnStateUpdate(float deltaTime) -> std::shared_ptr<IState> override;
    auto OnStateExit() -> void override {}

private:
    int m_CurrentPageItemIndex = {};
    std::shared_ptr<MenuItem> m_CurrentMenuItem = {};
    std::vector<std::shared_ptr<MenuItem>> m_MenuItems = {};

    std::shared_ptr<IState> m_CurrentState = {};
};