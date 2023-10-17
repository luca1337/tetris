#include <fsm/board/StateClear.h>
#include <Tetramino.h>
#include <Board.h>
#include <Input.h>
#include <Types.h>

StateClear::StateClear(const Window& window, Board& board) : FSM{window}, m_Board{board} { }

auto StateClear::OnStateEnter() -> void
{
    m_Board.m_IsBlinking = false;
}

auto StateClear::OnStateUpdate(float deltaTime) -> std::shared_ptr<IState>
{
    auto& boardMatrix = m_Board.Matrix();

    for (auto&& y : m_Board.RowsToClear())
    {
        // parto dalla colonna y-esima ed imposto 0 per il numero di colonne (-2 estremi esclusi) * sizeof(...)
        const auto rowSizeInBytes = sizeof(uint8_t) * (Columns - 2);
        auto srcIndex = (((y - 1) * Columns) + 1);
        auto destIndex = y * Columns + 1;

        // pulisco la riga corrente
        memset(&boardMatrix[destIndex], 0, rowSizeInBytes);

        // devo spostare tutte le righe partendo dalla y corrente - N fino ad arrivare all'inizio della board (0, 1)
        for (auto columnIdx = (y - 1); columnIdx != 0; columnIdx--)
        {
            // la formula e' y - 1 (per il source) e (y + 1) per il destination, per ogni riga da N ad N-1
            srcIndex = (columnIdx * Columns + 1);
            destIndex = ((columnIdx + 1) * Columns + 1);

            memmove(&boardMatrix[destIndex], &boardMatrix[srcIndex], rowSizeInBytes);
            memset(&boardMatrix[srcIndex], 0, rowSizeInBytes);
        }
    }

    OnStateExit();
    auto& nextState = GetState(StateType::Play);
    nextState->OnStateEnter();
    return nextState;
}

auto StateClear::OnStateExit() -> void
{
    m_Board.GetCurrentPlayingTetramino()->isVisible = true;
    m_Board.ShuffleCurrentPlayingTetramino();
    m_Board.RowsToClear().clear();
}