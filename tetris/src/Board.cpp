#include <Board.h>
#include <Texture.h>
#include <Tetramino.h>
#include <Input.h>

#include <ranges>
#include <algorithm>
#include <string>

#include <Random.h>

Board::Board(const Window &window) : m_Window{window}
{
    const auto sdlRenderer = static_cast<SDL_Renderer*>(m_Window.GetRendererHandle());

    m_TextureMap = std::unordered_map<uint8_t, std::shared_ptr<Texture>>
    {
        //{BackgroundIndex, std::make_shared<Texture>(sdlRenderer, "../../tetris/resources/textures/bg.png", true)},
        {BoundIndex, std::make_shared<Texture>(sdlRenderer, "../../tetris/resources/textures/gray.png", true)},
        {TetraminoIIndex, std::make_shared<Texture>(sdlRenderer, "../../tetris/resources/textures/cyan.png", true)},
        {TetraminoJIndex, std::make_shared<Texture>(sdlRenderer, "../../tetris/resources/textures/blue.png", true)},
        {TetraminoLIndex, std::make_shared<Texture>(sdlRenderer, "../../tetris/resources/textures/orange.png", true)},
        {TetraminoOIndex, std::make_shared<Texture>(sdlRenderer, "../../tetris/resources/textures/yellow.png", true)},
        {TetraminoSIndex, std::make_shared<Texture>(sdlRenderer, "../../tetris/resources/textures/green.png", true)},
        {TetraminoTIndex, std::make_shared<Texture>(sdlRenderer, "../../tetris/resources/textures/magenta.png", true)},
        {TetraminoZIndex, std::make_shared<Texture>(sdlRenderer, "../../tetris/resources/textures/red.png", true)},
    };

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
        m_Tetraminos.push_back(std::make_shared<Tetramino>(window, static_cast<TetraminoType>(tetraminoIdx)));
    });

    ShuffleCurrentPlayingTetramino();

    m_OriginalMatrix = m_Matrix;

    m_GhostTetramino = std::make_shared<Tetramino>(*m_CurrentTetramino);
}

auto Board::Draw() -> void
{
    const auto sdlRenderer = static_cast<SDL_Renderer*>(m_Window.GetRendererHandle());

    m_CurrentTetramino->Draw();

    m_GhostTetramino->Draw(0xFF*1/3);

    for (auto rowIdx = 0ul; rowIdx != Rows; ++rowIdx)
    {
        for (auto columnIdx = 0ul; columnIdx != Columns; ++columnIdx)
        {
            if (m_IsBlinking)
            {
                const auto foundIdx = std::find(m_RowsToClear.begin(), m_RowsToClear.end(), rowIdx);
                if (foundIdx != m_RowsToClear.end() && (columnIdx > 0 && columnIdx < Columns - 1))
                {
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

auto Board::ShuffleCurrentPlayingTetramino() -> void
{
    rng::RNG::ShuffleArray(m_Tetraminos);
    m_CurrentTetramino = m_Tetraminos[rng::RNG::GenerateRandomNumber(0, static_cast<int>(m_Tetraminos.size() - 1))];
}