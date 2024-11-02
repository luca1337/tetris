#pragma once

#include <rendering/Camera.h>

#include <memory>

class Window;
class Board;
class Menu;

class IState;

class StateSetup;

class StatePlay;
class StateCheck;
class StateBlink;
class StateClear;

class GameImpl
{
public:
    Camera m_MainCamera;
};

class Game
{
public:
    ~Game() { delete m_pImpl; }
    Game(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(const Game&) = delete;
    Game& operator=(Game&&) = delete;

    static auto GetInstance() -> Game&;

    auto Update() -> void;
    auto Init() -> void;
    auto SetGameState(const std::shared_ptr<IState>& state) { m_CurrentState = state; }

    auto GetMainCamera() const -> Camera& { return m_pImpl->m_MainCamera; }

private:
    Game() = default;

    std::shared_ptr<Board> m_Board = {};
    std::shared_ptr<Window> m_Context = {};

private:

    //  FSM current updated state
    std::shared_ptr<IState> m_CurrentState = {};

    // FSM (setup process)
    std::shared_ptr<StateSetup> m_SetupState = {};

    // FSM (board)
    std::shared_ptr<StatePlay> m_StatePlay = {};
    std::shared_ptr<StateCheck> m_StateCheck = {};
    std::shared_ptr<StateBlink> m_StateBlink = {};
    std::shared_ptr<StateClear> m_StateClear = {};

    std::shared_ptr<Menu> m_Menu = {};

    GameImpl* m_pImpl = {};
};