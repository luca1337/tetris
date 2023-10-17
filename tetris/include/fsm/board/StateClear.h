#pragma once

#include <fsm/base/FSM.h>
#include <Window.h>
#include <Board.h>

class StateClear : public FSM
{
public:
    ~StateClear() = default;
    StateClear(const Window& window, Board& board);

    auto OnStateEnter() -> void override;
    auto OnStateUpdate(float deltaTime) -> std::shared_ptr<IState> override;
    auto OnStateExit() -> void override;

private:
    Board& m_Board;
};