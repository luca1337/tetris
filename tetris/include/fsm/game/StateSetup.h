#pragma once

#include <fsm/base/FSM.h>
#include <Window.h>
#include <Board.h>

class RenderText;

class StateSetup : public FSM
{
public:
    ~StateSetup() = default;
    StateSetup(const Window& window);

    auto OnStateEnter() -> void override {}
    auto OnStateUpdate(float deltaTime) -> std::shared_ptr<IState> override;
    auto OnStateExit() -> void override {}

private:
    std::shared_ptr<RenderText> m_NintendoText = {};
    std::shared_ptr<RenderText> m_PressToSkipText = {};
    float m_ScrollTextTimer = {};
};