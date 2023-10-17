#pragma once

#include <fsm/base/FSM.h>
#include <Window.h>
#include <Board.h>

class StateBlink : public FSM
{
public:
    ~StateBlink() = default;
    StateBlink(const Window& window, Board& board);

    auto OnStateEnter() -> void override;
    auto OnStateUpdate(float deltaTime) -> std::shared_ptr<IState> override;
    auto OnStateExit() -> void override;

private:
    Board& m_Board;
    float m_BlinkTimer = {};
};