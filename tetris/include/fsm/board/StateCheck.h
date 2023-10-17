#pragma once

#include <fsm/base/FSM.h>
#include <Window.h>
#include <Board.h>

// Lo statePlay si occupa del movimento di base del tetramino (rotazione inclusa)

class StateCheck : public FSM
{
public:
    ~StateCheck() = default;
    StateCheck(const Window& window, Board& board);

    auto OnStateEnter() -> void override;
    auto OnStateUpdate(float deltaTime) -> std::shared_ptr<IState> override;
    auto OnStateExit() -> void override;

private:
    Board& m_Board;
};