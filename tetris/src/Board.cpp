#include <Board.h>
#include <Sprite.h>
#include <Tetramino.h>
#include <Input.h>
#include <StatsManager.h>
#include <Utils.h>

#include <ranges>
#include <algorithm>
#include <string>

#include <Random.h>

#include <rendering/Shader.h>
#include <ResourceManager.h>
#include <rendering/ParticleSystem.h>

constexpr int X_UNIT_PADDING = 9;
constexpr int Y_UNIT_PADDING = 15;

Board::Board(const Window &window) : m_Window{window}
{
    const auto particleShader = std::make_shared<Shader>("../../tetris/resources/shaders/vertex_particle.glsl", "../../tetris/resources/shaders/fragment_particle.glsl");
    const auto fontShader = std::make_shared<Shader>("../../tetris/resources/shaders/vertex_font.glsl", "../../tetris/resources/shaders/fragment_font.glsl");
    const auto spriteShader = std::make_shared<Shader>("../../tetris/resources/shaders/vertex_sprite.glsl", "../../tetris/resources/shaders/fragment_sprite.glsl");

    ResourceManager::RegisterResource(ResourceParams{ResourceType::Shader, "ParticleShader", particleShader});
    ResourceManager::RegisterResource(ResourceParams{ResourceType::Shader, "FontShader", fontShader});
    ResourceManager::RegisterResource(ResourceParams{ResourceType::Shader, "SpriteShader", spriteShader});

    m_StatsBgTexture = std::make_shared<Sprite>("../../tetris/resources/textures/tetris_stats_bg.png");
    if (!m_StatsBgTexture)
    {
        SDL_Log("Couldn't create Stats Background Texture..");
        return;
    }

    // todo: fix background stuff
    const auto bgSize = m_StatsBgTexture->GetSize();
    const auto bgPos = m_StatsBgTexture->GetPositionOnScreen();

    m_StatsBgTexture->SetLocalTranslation((bgPos.x + window.Props().width - bgSize.x), bgPos.y);

    for (auto rowIdx = 0ul; rowIdx != Rows; ++rowIdx)
    {
        for (auto columnIdx = 0ul; columnIdx != Columns; ++columnIdx)
        {
            if (columnIdx == 0 || columnIdx == (Columns - 1))
            {
                auto wallBlock = std::make_shared<Sprite>("../../tetris/resources/textures/gray.png");
                wallBlock->SetLocalTranslation(columnIdx * BlockSize, rowIdx * BlockSize);
                m_WallBlocks.push_back(wallBlock);
                m_Matrix.push_back(BoundIndex);
            }
            else
            {
                m_Matrix.push_back(BackgroundIndex);
            }
        }
    }

    std::ranges::for_each(std::views::iota(0ul) | std::views::take(MaxNumTetraminos), [&](const int tetraminoIdx) {
        m_Tetraminos.push_back(std::make_shared<Tetramino>(window, static_cast<TetraminoType>(tetraminoIdx)));
    });

    m_NextTetramino = std::make_shared<Tetramino>(*m_Tetraminos[rng::RNG::GenerateRandomNumber(0, static_cast<int>(m_Tetraminos.size() - 1))]);
    for (auto& block: m_NextTetramino->Blocks())
    {
        const auto positionOnScreen = block->GetPositionOnScreen();
        block->SetLocalTranslation(positionOnScreen.x + BlockSize * X_UNIT_PADDING, positionOnScreen.y + BlockSize * Y_UNIT_PADDING);
    }

    
    RandomizeCurrentPlayingTetramino();

    m_OriginalMatrix = m_Matrix;

    m_GhostTetramino = std::make_shared<Tetramino>(*m_CurrentTetramino);
    m_GhostTetramino->SetAlpha(0.5f);

    m_StatsManager = std::make_shared<StatsManager>(window);

    m_DestroyBlockPSs = std::vector<std::shared_ptr<ParticleSystem>>{};
}

auto Board::Draw() -> void
{
    // todo: this must be optimized, maybe cached outside of this loop
    const auto spriteShader = ResourceManager::GetFromCache<Shader>({ResourceType::Shader, "SpriteShader"});
    const auto particleShader = ResourceManager::GetFromCache<Shader>({ResourceType::Shader, "ParticleShader"});

#pragma region UI
    for (auto&& ps: m_DestroyBlockPSs)
    {
        ps->Update(m_Window.m_DeltaTime);
        ps->Render(particleShader.value());
    }

    m_StatsBgTexture->Render(spriteShader.value());

    m_StatsManager->Draw();

#pragma endregion

#pragma region TETRAMINOS

    m_CurrentTetramino->Draw();
    m_NextTetramino->Draw();

    m_GhostTetramino->SetAlpha(0.3f);
    m_GhostTetramino->Draw();

#pragma endregion

    for (auto rowIdx = 0ul; rowIdx != Rows; ++rowIdx)
    {
        for (auto columnIdx = 0ul; columnIdx != Columns; ++columnIdx)
        {
            // Skip blocks that are blinking
            if (m_IsBlinking)
            {
                const auto foundIdx = std::find(m_RowsToClear.begin(), m_RowsToClear.end(), rowIdx);
                if (foundIdx != m_RowsToClear.end() && utils::IsValueBetween(static_cast<int>(columnIdx), 1, Columns - 2)) // skip borders (gray walls)
                {
                    continue;
                }
            }

            const auto index = rowIdx * Columns + columnIdx;
            if (m_SpriteMap.contains(index))
            {
                auto& block = m_SpriteMap[index];
                block->SetLocalTranslation(columnIdx * BlockSize, rowIdx * BlockSize);
                block->Render(spriteShader.value());
            }

            // Draw Walls
            for (const auto& wall: m_WallBlocks)
            {
                wall->Render(spriteShader.value());
            }
        }
    }

}

auto Board::RandomizeCurrentPlayingTetramino() -> void
{
    rng::RNG::ShuffleArray(m_Tetraminos);

    m_CurrentTetramino = std::make_shared<Tetramino>(*m_NextTetramino);
    m_CurrentTetramino->Reset();

    m_NextTetramino = std::make_shared<Tetramino>(*m_Tetraminos[rng::RNG::GenerateRandomNumber(0, static_cast<int>(m_Tetraminos.size() - 1))]);
    for (auto& block: m_NextTetramino->Blocks())
    {
        const auto positionOnScreen = block->GetPositionOnScreen();
        block->SetLocalTranslation(positionOnScreen.x + BlockSize * X_UNIT_PADDING, positionOnScreen.y + BlockSize * Y_UNIT_PADDING);
    }
}