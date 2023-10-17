#pragma once

#include <fsm/base/IState.h>
#include <Window.h>
#include <Types.h>

#include <memory>

class FSM : public IState
{
public:
    FSM(const Window& window);

    auto OnStateEnter() -> void override;
    auto OnStateUpdate(float deltaTime) -> std::shared_ptr<IState> override;
    auto OnStateExit() -> void override;

    auto SetState(const StateType, const std::shared_ptr<IState>&) -> void;
    auto& GetState(const StateType type) { return m_StatesMap[type]; }

protected:
    const Window& m_Window;

private:
    std::unordered_map<StateType, std::shared_ptr<IState>> m_StatesMap = {};
};