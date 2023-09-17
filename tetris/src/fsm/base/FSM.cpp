#include <fsm/base/FSM.h>

FSM::FSM(const Window& window) : m_Window{window} { }

auto FSM::OnStateEnter() -> void { }

auto FSM::OnStateUpdate() -> std::shared_ptr<IState> { return shared_from_this(); }

auto FSM::OnStateExit() -> void { }
