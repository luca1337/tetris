#include <fsm/board/StatePlay.h>
#include <tetraminos/Tetramino.h>
#include <Board.h>
#include <Input.h>
#include <Types.h>

StatePlay::StatePlay(const Window& window, const Board& board) : FSM{window}, m_Board{board}
{

}

auto StatePlay::OnStateEnter() -> void
{
}

auto StatePlay::OnStateUpdate() -> std::shared_ptr<IState>
{
    const auto& currentTetramino = m_Board.GetCurrentPlayingTetramino();

    if (currentTetramino && canMove)
    {
        if (Input::IsKeyPressed(m_Window, SDLK_d) && currentTetramino->CanMove(m_Matrix, DirectionType::Right))
        {
            currentTetramino->Translate(DirectionType::Right);
        }

        if (Input::IsKeyPressed(m_Window, SDLK_a) && currentTetramino->CanMove(m_Matrix, DirectionType::Left))
        {
            currentTetramino->Translate(DirectionType::Left);
        }

        if (Input::IsKeyPressed(m_Window, SDLK_SPACE))
        {
            currentTetramino->Rotate(m_Matrix, DirectionType::Right);
        }
    }

    return shared_from_this();
}

auto StatePlay::OnStateExit() -> void
{
    return;
}
