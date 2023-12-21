#include "StatsManager.h"

#include <Constants.h>
#include <Color.h>
#include <Window.h>
#include <RenderText.h>

StatsManager::StatsManager(const Window& window) : m_Window{window}
{
    m_ScoreText = std::make_shared<RenderText>(window.GetRendererHandle(), "../../tetris/resources/fonts/tetris.ttf", 64);
    if (!m_ScoreText)
    {
        SDL_Log("Couldn't create text Score Text UI.");
        return;
    }

    m_LevelText = std::make_shared<RenderText>(window.GetRendererHandle(), "../../tetris/resources/fonts/tetris.ttf", 64);
    if (!m_LevelText)
    {
        SDL_Log("Couldn't create text Level Text UI.");
        return;
    }

    m_LinesText = std::make_shared<RenderText>(window.GetRendererHandle(), "../../tetris/resources/fonts/tetris.ttf", 64);
    if (!m_LinesText)
    {
        SDL_Log("Couldn't create text Lines Text UI.");
        return;
    }

    m_ScoreText->SetColor(Color{127, 85, 188, 255});
    m_ScoreText->Update("0");

    m_LevelText->SetColor(Color{127, 85, 188, 255});
    m_LevelText->Update("0");

    m_LinesText->SetColor(Color{127, 85, 188, 255});
    m_LinesText->Update("0");
}

auto StatsManager::UpdateScoreboard(const ScoreboardTextType textType, const unsigned newScore) -> void
{
    const auto scoreToString = std::to_string(newScore);
    
    switch (textType)
    {
    case ScoreboardTextType::Score:
        m_ScoreText->Update(scoreToString);
        break;
    case ScoreboardTextType::Level:
        m_LevelText->Update(scoreToString);
        break;
    case ScoreboardTextType::Lines:
        m_LinesText->Update(scoreToString);
        break;
    default:
        break;
    }
}

auto StatsManager::Draw() -> void
{
    m_ScoreText->Draw(BlockSize * 14, BlockSize * 5);
    m_LevelText->Draw(BlockSize * 14, (BlockSize * 9) + 12);
    m_LinesText->Draw(BlockSize * 14, (BlockSize * 13) + 3);
}
