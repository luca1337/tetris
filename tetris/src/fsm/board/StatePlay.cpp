#include <fsm/board/StatePlay.h>
#include <Tetramino.h>
#include <Board.h>
#include <Input.h>
#include <Types.h>

StatePlay::StatePlay(const Window& window, Board& board) : FSM{window}, m_Board{board}, m_PlaceTimer{0.5f}, m_MoveDownTimer{0.2f} { }

auto StatePlay::OnStateEnter() -> void
{
    SDL_Log("Entering in State Play");

    m_PlaceTimer.Reset();
    m_MoveDownTimer.Reset();
    m_ShouldMoveDown = true;
    m_ShouldRecomputePreview = true;
    m_Board.RowsToClear().clear();
    m_NextState = shared_from_this();
}

auto StatePlay::OnStateUpdate(float deltaTime) -> std::shared_ptr<IState>
{
    auto& currentTetramino = m_Board.GetCurrentPlayingTetramino();
    auto& currentTetraminoPreview = m_Board.GetGhostTetramino();
    auto& boardMatrix = m_Board.Matrix();

    if (m_ShouldMoveDown)
    {
        m_MoveDownTimer.Update(deltaTime, nullptr, [&](const auto elapsed){
            if (currentTetramino->CanMove(boardMatrix, DirectionType::Down))
            {
                currentTetramino->Translate(DirectionType::Down);
                m_MoveDownTimer.Reset();
            }
            else
            {
                m_ShouldMoveDown = false;
                m_MoveDownTimer.Reset();
            }
        });
    }
    else
    {
        m_PlaceTimer.Update(deltaTime, nullptr, [&](const auto elapsed){
            this->OnStateExit();

            auto& nextState = GetState(StateType::Check);
            nextState->OnStateEnter();
            m_NextState = nextState;
        });
    }

    if (currentTetramino)
    {
        if (Input::IsKeyPressed(m_Window, SDLK_d) && currentTetramino->CanMove(boardMatrix, DirectionType::Right))
        {
            currentTetramino->Translate(DirectionType::Right);
            m_ShouldMoveDown = currentTetramino->CanMove(boardMatrix, DirectionType::Down);
            m_ShouldRecomputePreview = true;
        }

        if (Input::IsKeyPressed(m_Window, SDLK_a) && currentTetramino->CanMove(boardMatrix, DirectionType::Left))
        {
            currentTetramino->Translate(DirectionType::Left);
            m_ShouldMoveDown = currentTetramino->CanMove(boardMatrix, DirectionType::Down);
            m_ShouldRecomputePreview = true;
        }

        if (Input::IsKeyPressed(m_Window, SDLK_s) && currentTetramino->CanMove(boardMatrix, DirectionType::Down))
        {
            currentTetramino->Translate(DirectionType::Down);
            m_ShouldMoveDown = currentTetramino->CanMove(boardMatrix, DirectionType::Down);
        }

        if (Input::IsKeyPressed(m_Window, SDLK_SPACE))
        {
            currentTetramino->Rotate(boardMatrix, DirectionType::Right);
            m_ShouldMoveDown = currentTetramino->CanMove(boardMatrix, DirectionType::Down);
            m_ShouldRecomputePreview = true;
        }
    }

    if (m_ShouldRecomputePreview)
    {
        currentTetraminoPreview = std::make_shared<Tetramino>(*currentTetramino);

        while (currentTetraminoPreview->CanMove(boardMatrix, DirectionType::Down))
        {
            currentTetraminoPreview->Translate(DirectionType::Down);
        }
        
        m_ShouldRecomputePreview = false;
    }

    return m_NextState;
}

auto StatePlay::OnStateExit() -> void 
{

}