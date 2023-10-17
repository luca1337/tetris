#pragma once

#include <memory>

class Window;
class Board;

class IState;

class StateSetup;
class StateMainMenu;

class StatePlay;
class StateCheck;
class StateBlink;
class StateClear;

class Game
{
public:
    ~Game() = default;
    Game();

    auto Update() -> void;
    auto SetGameState(const std::shared_ptr<IState>& state) 
    { 
        m_CurrentState = state; 
    }

private:
    std::shared_ptr<Board> m_Board = {};
    std::shared_ptr<Window> m_Context = {};

private:

    //  FSM current updated state
    std::shared_ptr<IState> m_CurrentState = {};

    // FSM (setup process)
    std::shared_ptr<StateSetup> m_SetupState = {};
    std::shared_ptr<StateMainMenu> m_MainMenuState = {};

    // FSM (board)
    std::shared_ptr<StatePlay> m_StatePlay = {};
    std::shared_ptr<StateCheck> m_StateCheck = {};
    std::shared_ptr<StateBlink> m_StateBlink = {};
    std::shared_ptr<StateClear> m_StateClear = {};
};