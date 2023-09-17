#include <fsm/base/FSM.h>
#include <Window.h>
#include <Board.h>

class StatePlay : public FSM
{
public:
    ~StatePlay() = default;
    StatePlay(const Window& window, const Board& board);

    auto OnStateEnter() -> void override;
    auto OnStateUpdate() -> std::shared_ptr<IState> override;
    auto OnStateExit() -> void override;

private:
    Board m_Board;
};