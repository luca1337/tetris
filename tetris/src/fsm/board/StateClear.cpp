#include <fsm/board/StateClear.h>
#include <Tetramino.h>
#include <Board.h>
#include <Input.h>
#include <Types.h>
#include <Sprite.h>

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
        for (size_t x = 1; x != Columns - 1; x++)
        {
            auto currentColumnIdx = y * Columns + x;
            
            if (m_Board.GetSpriteMap().contains(currentColumnIdx))
            {
                m_Board.GetSpriteMap().erase(currentColumnIdx);
            }
        }
    }

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

    for (auto&& y : m_Board.RowsToClear())
    {
        for (auto columnIdx = y; columnIdx != 0; columnIdx--)
        {
            for (auto x = 1UL; x != Columns - 1; x++)
            {
                auto srcIndex = (columnIdx * Columns + x);
                auto destIndex = ((columnIdx + 1) * Columns + x);

                if (m_Board.GetSpriteMap().contains(srcIndex))
                {
                    auto sprite = m_Board.GetSpriteMap()[srcIndex];
                    m_Board.GetSpriteMap().erase(srcIndex);
                    sprite->SetLocalTranslation(x * BlockSize, (columnIdx + 1) * BlockSize);
                    m_Board.GetSpriteMap()[destIndex] = sprite;
                }
            }
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
    m_Board.RandomizeCurrentPlayingTetramino();
    m_Board.RowsToClear().clear();
}