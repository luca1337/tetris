#include <Board.h>
#include <Texture.h>
#include <tetraminos/Tetramino.h>
#include <Input.h>
#include <fsm/board/StatePlay.h>

#include <ranges>
#include <algorithm>
#include <string>

#include <rng.h>

Board::Board(const Window &window) : m_Window{window}
{
    const auto sdlRenderer = static_cast<SDL_Renderer*>(m_Window.GetRendererHandle());

    m_TextureMap = std::unordered_map<uint8_t, std::shared_ptr<Texture>>
    {
        {BoundIndex, std::make_shared<Texture>(sdlRenderer, BlockSize, BlockSize, Gray)},
        // ---
        {TetraminoIIndex, std::make_shared<Texture>(sdlRenderer, BlockSize, BlockSize, Cyan)},
        {TetraminoJIndex, std::make_shared<Texture>(sdlRenderer, BlockSize, BlockSize, Blue)},
        {TetraminoLIndex, std::make_shared<Texture>(sdlRenderer, BlockSize, BlockSize, Orange)},
        {TetraminoOIndex, std::make_shared<Texture>(sdlRenderer, BlockSize, BlockSize, Yellow)},
        {TetraminoSIndex, std::make_shared<Texture>(sdlRenderer, BlockSize, BlockSize, Green)},
        {TetraminoTIndex, std::make_shared<Texture>(sdlRenderer, BlockSize, BlockSize, Magenta)},
        {TetraminoZIndex, std::make_shared<Texture>(sdlRenderer, BlockSize, BlockSize, Red)},
    };

    // Primo passo è quello di mettere i muri a lato della board e salvarli nella matrice iniziale con le relative posizioni
    // viene ciclato righe per colonne cosi saranno disposti correttamente nell'array monodimensionale (come se fosse bidimensionale)
    for (auto rowIdx = 0ul; rowIdx != Rows; ++rowIdx)
    {
        for (auto columnIdx = 0ul; columnIdx != Columns; ++columnIdx)
        {
            if (columnIdx == 0 || columnIdx == (Columns - 1))
            {
                m_Matrix.push_back(BoundIndex);
            }
            else
            {
                m_Matrix.push_back(BackgroundIndex);
            }
        }
    }

    std::ranges::for_each(std::views::iota(0ul) | std::views::take(MaxNumTetraminos), [&](const int tetraminoIdx) {
        m_Tetraminos[tetraminoIdx] = std::make_shared<Tetramino>(window, static_cast<TetraminoType>(tetraminoIdx));
    });

    m_CurrentTetramino = GetRandomTetramino();

    m_OriginalMatrix = m_Matrix;

    m_StatePlay = std::make_shared<StatePlay>(window, *this);
}

float moveTimer = {};
float blockedTimer = {};
float blinkTimer = {};
bool shouldStartBlinkTimer = {};
bool shouldMoveDown = true;
bool shouldCheckForLineComplete = {};
bool canMove = true;
int rowIndexToClear = {};
bool isBlinking = false;
std::vector<int> rowsToClear = {};

auto Board::Update(float deltaTime) -> void
{
    const auto sdlRenderer = static_cast<SDL_Renderer*>(m_Window.GetRendererHandle());

    if (shouldMoveDown)
    {
        moveTimer += deltaTime;

        if ( (moveTimer >= 0.2f) )
        {
            if (m_CurrentTetramino->CanMove(m_Matrix, DirectionType::Down))
            {
                m_CurrentTetramino->Translate(DirectionType::Down);
                moveTimer = 0.0f;
            }
            else
            {
                shouldMoveDown = false;
                shouldCheckForLineComplete = true;
                moveTimer = 0.0f;
            }
        }
    }

    if (shouldCheckForLineComplete)
    {
        blockedTimer += deltaTime;

        if (blockedTimer >= .5f)
        {
            canMove = false;

            for(const auto& blockIdx: m_CurrentTetramino->GetBlockPositionInMatrix())
            {
                m_Matrix[blockIdx] = m_CurrentTetramino->GetIndex();
                m_CurrentTetramino->Reset();
            }

            uint8_t count = {};

            // ogni volta che piazzo un tetramino devo controllare la riga se e' completa oppure no

            for (auto y = 0ul; y != Rows; ++y)
            {
                for (auto x = 0ul; x != Columns; ++x)
                {
                    // y * CumColonne + x (l'array viene iterato da sx a dx dall'alto verso il basso)
                    const auto index = y * Columns + x;

                    if (m_Matrix[index] != 1 && m_Matrix[index] != 0)
                    {
                        count++;
                    }
                }

                if (count == (Columns - 2))
                {
                    shouldStartBlinkTimer = true;
                    rowsToClear.push_back(y);
                }
            
                count = 0;
            }

            if (rowsToClear.size() == 0)
            {
                canMove = true;
                shouldMoveDown = true;
                m_CurrentTetramino = GetRandomTetramino();
            }
            else
            {
                shouldStartBlinkTimer = true;
            }

            shouldCheckForLineComplete = false;
            blockedTimer = 0.0f;
        }
    }

    if (shouldStartBlinkTimer)
    {
        blinkTimer += deltaTime;

        if (blinkTimer >= 2.f)
        {
            isBlinking = false;

            for (auto&& y : rowsToClear)
            {
                // parto dalla colonna y-esima ed imposto 0 per il numero di colonne (-2 estremi esclusi) * sizeof(...)
                const auto rowSizeInBytes = sizeof(uint8_t) * (Columns - 2);
                auto srcIndex = (((y - 1) * Columns) + 1);
                auto destIndex = y * Columns + 1;

                // pulisco la riga corrente
                memset(&m_Matrix[destIndex], 0, rowSizeInBytes);

                // devo spostare tutte le righe partendo dalla y corrente - N fino ad arrivare all'inizio della board (0, 1)
                for (auto columnIdx = (y - 1); columnIdx != 0; columnIdx--)
                {
                    // la formula e' y - 1 (per il source) e (y + 1) per il destination, per ogni riga da N ad N-1
                    srcIndex = (columnIdx * Columns + 1);
                    destIndex = ((columnIdx + 1) * Columns + 1);

                    memmove(&m_Matrix[destIndex], &m_Matrix[srcIndex], rowSizeInBytes);
                    memset(&m_Matrix[srcIndex], 0, rowSizeInBytes);
                }
            }

            canMove = true;
            shouldMoveDown = true;
            shouldStartBlinkTimer = false;
            m_CurrentTetramino = GetRandomTetramino();
            rowsToClear.clear();
            blinkTimer = 0.0f;
        }
        else
        {
            isBlinking = true;

            for (auto&& y : rowsToClear)
            {
                for (auto columnIdx = 1ul; columnIdx != (Columns - 1); columnIdx++)
                {
                    int index = y * Columns + columnIdx;

                    auto block = m_TextureMap[m_Matrix[index]];

                    block->SetPositionOnScreen(columnIdx * BlockSize, y * BlockSize);
                    const auto alphaMod = static_cast<uint8_t>(abs((255 * sin(blinkTimer) / pow(blinkTimer, 2))));
                    block->Draw(sdlRenderer, alphaMod);
                }
            }
        }
    }

    if (m_CurrentTetramino && canMove)
    {
        if (Input::IsKeyPressed(m_Window, SDLK_d) && m_CurrentTetramino->CanMove(m_Matrix, DirectionType::Right))
        {
            m_CurrentTetramino->Translate(DirectionType::Right);
        }

        if (Input::IsKeyPressed(m_Window, SDLK_a) && m_CurrentTetramino->CanMove(m_Matrix, DirectionType::Left))
        {
            m_CurrentTetramino->Translate(DirectionType::Left);
        }

        if (Input::IsKeyPressed(m_Window, SDLK_SPACE))
        {
            m_CurrentTetramino->Rotate(m_Matrix, DirectionType::Right);
        }
    }
}

auto Board::Draw() -> void
{
    const auto sdlRenderer = static_cast<SDL_Renderer*>(m_Window.GetRendererHandle());

    m_CurrentTetramino->Draw();

    int idx = 0;

    for (auto rowIdx = 0ul; rowIdx != Rows; ++rowIdx)
    {

        for (auto columnIdx = 0ul; columnIdx != Columns; ++columnIdx)
        {

            if (isBlinking)
            {
                // Verifica se rowIdx è nell'elenco degli indici da saltare e se columnIdx è nell'intervallo desiderato
                auto foundIdx = std::find(rowsToClear.begin(), rowsToClear.end(), rowIdx);
                if (foundIdx != rowsToClear.end() && (columnIdx > 0 && columnIdx < Columns - 1))
                {
                    // Questa riga è nell'elenco degli indici da saltare o è fuori dall'intervallo desiderato, passa alla successiva
                    continue;
                }
            }

            const auto index = rowIdx * Columns + columnIdx;

            if (m_TextureMap.contains(m_Matrix[index]))
            {
                auto& block = m_TextureMap[m_Matrix[index]];
                block->SetPositionOnScreen(columnIdx * BlockSize, rowIdx * BlockSize);
                block->Draw(sdlRenderer, 0xFF);
            }
        }
    }
}

auto Board::GetCurrentPlayingTetramino() const -> std::shared_ptr<Tetramino>
{
    return m_CurrentTetramino;
}

auto Board::GetRandomTetramino() -> std::shared_ptr<Tetramino>&
{
    return m_Tetraminos[rng::RNG::GenerateRandomNumber(0, static_cast<int>(m_Tetraminos.size() - 1))];
}