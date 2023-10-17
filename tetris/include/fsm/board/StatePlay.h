#pragma once

#include <fsm/base/FSM.h>
#include <Window.h>
#include <Board.h>
#include <Timer.h>

class StatePlay : public FSM
{
public:
    ~StatePlay() = default;
    StatePlay(const Window& window, Board& board);

    auto OnStateEnter() -> void override;
    auto OnStateUpdate(float deltaTime) -> std::shared_ptr<IState> override;
    auto OnStateExit() -> void override;

private:
    Board& m_Board;

    Timer m_PlaceTimer = {};
    Timer m_MoveDownTimer = {};

    bool m_ShouldMoveDown = {};
    bool m_ShouldRecomputePreview = true;

    std::shared_ptr<IState> m_NextState = {};
};