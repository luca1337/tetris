#include <fsm/board/StateCheck.h>
#include <Tetramino.h>
#include <Board.h>
#include <Input.h>
#include <Types.h>

StateCheck::StateCheck(const Window& window, Board& board) : FSM{window}, m_Board{board} { }

auto StateCheck::OnStateEnter() -> void { }

auto StateCheck::OnStateUpdate(float deltaTime) -> std::shared_ptr<IState>
{
    const auto& currentTetramino = m_Board.GetCurrentPlayingTetramino();
    const auto& boardMatrix = m_Board.Matrix();

    // piazzo il tetramino
    for(const auto& blockIdx: currentTetramino->GetBlockPositionInMatrix())
    {
        m_Board.Matrix()[blockIdx] = currentTetramino->GetIndex();
        currentTetramino->Reset();
    }

    auto completedColumn = uint8_t{};

    // ogni volta che piazzo un tetramino devo controllare la riga se e' completa oppure no

    for (auto y = 0ul; y != Rows; ++y)
    {
        for (auto x = 0ul; x != Columns; ++x)
        {
            // y * CumColonne + x (l'array viene iterato da sx a dx dall'alto verso il basso)
            const auto index = y * Columns + x;

            if (m_Board.Matrix()[index] != 1 && m_Board.Matrix()[index] != 0)
            {
                completedColumn++;
            }
        }

        if (completedColumn == (Columns - 2))
        {
            m_Board.RowsToClear().push_back(y);
        }
    
        completedColumn = 0;
    }

    // ora controllo se ci sono delle linee da pulire, se ci sono devo fare l'effetto blink prima di eliminarle ( stato successivo )

    if (!m_Board.RowsToClear().empty())
    {
        auto& nextState = GetState(StateType::Blink);
        nextState->OnStateEnter();
        currentTetramino->isVisible = false;
        return nextState;
    }

    OnStateExit();
    auto& nextState = GetState(StateType::Play);
    nextState->OnStateEnter();
    return nextState;
}

auto StateCheck::OnStateExit() -> void
{
    m_Board.ShuffleCurrentPlayingTetramino();
}