#pragma once

#include <fsm/base/IState.h>
#include <Window.h>

#include <memory>

class FSM : public IState
{
public:
    FSM(const Window& window);

    auto OnStateEnter() -> void override;
    auto OnStateUpdate() -> std::shared_ptr<IState> override;
    auto OnStateExit() -> void override;

    auto& State() { return m_State; }
    const auto& State() const { return m_State; }

protected:
    Window m_Window = {};

private:
    std::shared_ptr<IState> m_State = {};
};