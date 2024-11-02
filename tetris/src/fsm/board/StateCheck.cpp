#include <fsm/board/StateCheck.h>
#include <Tetramino.h>
#include <Board.h>
#include <Input.h>
#include <Types.h>
#include <rendering/ParticleSystem.h>

#include <Sprite.h>

struct BBox{
    glm::vec2 m_Min = {};
    glm::vec2 m_Max = {};
};

std::shared_ptr<Sprite> CreateSprite(uint8_t index) 
{
    switch (index) {
        case TetraminoIIndex:
            return std::make_shared<Sprite>("../../tetris/resources/textures/cyan.png");
        case TetraminoJIndex:
            return std::make_shared<Sprite>("../../tetris/resources/textures/blue.png");
        case TetraminoLIndex:
            return std::make_shared<Sprite>("../../tetris/resources/textures/orange.png");
        case TetraminoOIndex:
            return std::make_shared<Sprite>("../../tetris/resources/textures/yellow.png");
        case TetraminoSIndex:
            return std::make_shared<Sprite>("../../tetris/resources/textures/green.png");
        case TetraminoTIndex:
            return std::make_shared<Sprite>("../../tetris/resources/textures/magenta.png");
        case TetraminoZIndex:
            return std::make_shared<Sprite>("../../tetris/resources/textures/red.png");
        default:
            return nullptr; // o un placeholder
    }
}

auto CreateAndAddParticleSystem(const BBox& particleBBox, Board& board) -> void
{
    auto constexpr particlesCount = 1000;
    auto constexpr particlesMinSpeed = 10.7f;
    auto constexpr particlesMaxSpeed = 25.4f;
    auto constexpr particlesMinLifeTime = 0.7f;
    auto constexpr particlesMaxLifeTime = 1.0f;

    const auto centerX = (particleBBox.m_Min.x + particleBBox.m_Max.x) / 2.0f;
    const auto centerY = (particleBBox.m_Min.y + particleBBox.m_Max.y) / 2.0f;

    const auto width = particleBBox.m_Max.x - particleBBox.m_Min.x;
    const auto height = particleBBox.m_Max.y - particleBBox.m_Min.y;

    const auto spawnPos = glm::vec2(centerX + (BlockSize / 2), centerY);

    const auto minSpawnX = -((width / 2.0f) + BlockSize / 2);
    const auto maxSpawnX = (width / 2.0f) + BlockSize / 2;

    const auto minSpawnY = -height / 2.0f;
    const auto maxSpawnY = height / 2.0f;

    auto ps = std::make_shared<ParticleSystem>(spawnPos, particlesCount, particlesMinSpeed, particlesMaxSpeed, particlesMinLifeTime, particlesMaxLifeTime);
    ps->Expire() = true;
    ps->RandomizeParticlesPositionBetweenBounds(minSpawnX, maxSpawnX, minSpawnY, maxSpawnY);
    ps->Emitting() = true;

    board.AddParticleSystem(ps);
}

StateCheck::StateCheck(const Window& window, Board& board) : FSM{window}, m_Board{board} { }

auto StateCheck::OnStateEnter() -> void 
{

}

auto StateCheck::OnStateUpdate(float deltaTime) -> std::shared_ptr<IState>
{
    m_Board.ClearParticleSystemCache();

    const auto& currentTetramino = m_Board.GetCurrentPlayingTetramino();
    const auto& boardMatrix = m_Board.Matrix();

    // piazzo il tetramino
    for(const auto& blockIdx: currentTetramino->GetBlockPositionInMatrix())
    {
        m_Board.Matrix()[blockIdx] = currentTetramino->GetIndex();
        currentTetramino->Reset();

        m_Board.GetSpriteMap()[blockIdx] = CreateSprite(currentTetramino->GetIndex());
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

    auto particleBBox = BBox{
        { std::numeric_limits<float>::max(), std::numeric_limits<float>::max() },
        { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest() },
    };

    auto& rowsToClear = m_Board.RowsToClear();
    std::reverse(rowsToClear.begin(), rowsToClear.end());
    const auto numOfRowsToClear = rowsToClear.size();

    if (!rowsToClear.empty())
    {
        auto nonContiguousLines = std::vector<unsigned>{};

        auto bboxCount = 0;
        auto lastRow = 0;

        if (numOfRowsToClear > 1)
        {
            nonContiguousLines.push_back(rowsToClear[0]);

            for (auto i = 1ul; i < numOfRowsToClear; ++i)
            {
                const auto prev = rowsToClear[i - 1];
                const auto next = rowsToClear[i];
                const auto diff = next - prev;

                if (diff > 1)
                { 
                    nonContiguousLines.push_back(next);
                }
                else
                {
                    if (nonContiguousLines.back() == prev)
                    {
                        nonContiguousLines.pop_back();
                    }
                }
            }

            auto CheckContinuity = [&](const auto row) { return std::find(nonContiguousLines.begin(), nonContiguousLines.end(), row) == nonContiguousLines.end(); };
            
            for (auto i = 0ul; i != numOfRowsToClear; ++i)
            {
                const auto currentRow = rowsToClear[i];

                if (!CheckContinuity(currentRow))
                {
                    if (bboxCount > 0)
                    {
                        particleBBox = 
                        {
                            {BlockSize, lastRow * BlockSize},
                            {BlockSize * (Columns - 2), (lastRow * BlockSize) + (bboxCount * BlockSize)}
                        };

                        CreateAndAddParticleSystem(particleBBox, m_Board);

                        bboxCount = {};
                    }
                    else
                    {
                        particleBBox = 
                        {
                            {BlockSize, currentRow * BlockSize},
                            {BlockSize * (Columns - 2), (currentRow * BlockSize) + BlockSize}
                        };

                        CreateAndAddParticleSystem(particleBBox, m_Board);
                    }

                    particleBBox = {};
                }
                else
                {
                    lastRow = currentRow;
                    bboxCount++;
                }
            }
        }
        else
        {
            particleBBox = 
            {
                {BlockSize, rowsToClear[0] * BlockSize},
                {BlockSize * (Columns - 2), (rowsToClear[0] * BlockSize) + BlockSize}
            };

            CreateAndAddParticleSystem(particleBBox, m_Board);
        }

        if (bboxCount > 0)
        {
            particleBBox = 
            {
                {BlockSize, lastRow * BlockSize},
                {BlockSize * (Columns - 2), (lastRow * BlockSize) + (bboxCount * BlockSize)}
            };

            CreateAndAddParticleSystem(particleBBox, m_Board);

            bboxCount = {};
        }

        std::reverse(rowsToClear.begin(), rowsToClear.end());
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
    m_Board.RandomizeCurrentPlayingTetramino();
}