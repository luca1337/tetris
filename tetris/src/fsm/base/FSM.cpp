#include <fsm/base/FSM.h>

FSM::FSM(const Window& window) : m_Window{window} { }

auto FSM::OnStateEnter() -> void { }

auto FSM::OnStateUpdate(float deltaTime) -> std::shared_ptr<IState> { return shared_from_this(); }

auto FSM::OnStateExit() -> void {}

auto FSM::SetState(const StateType type, const std::shared_ptr<IState>& state) -> void
{
    m_StatesMap[type] = state;
}
