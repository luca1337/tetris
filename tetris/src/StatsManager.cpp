#include "StatsManager.h"

#include <Constants.h>
#include <Color.h>
#include <Window.h>
#include <RenderText.h>
#include <ResourceManager.h>
#include <rendering/Shader.h>

StatsManager::StatsManager(const Window &window) : m_Window{window}
{
    m_ScoreText = std::make_shared<RenderText>("../../tetris/resources/fonts/tetris.ttf", 64);
    if (!m_ScoreText)
    {
        SDL_Log("Couldn't create text Score Text UI.");
        return;
    }

    m_LevelText = std::make_shared<RenderText>("../../tetris/resources/fonts/tetris.ttf", 64);
    if (!m_LevelText)
    {
        SDL_Log("Couldn't create text Level Text UI.");
        return;
    }

    m_LinesText = std::make_shared<RenderText>("../../tetris/resources/fonts/tetris.ttf", 64);
    if (!m_LinesText)
    {
        SDL_Log("Couldn't create text Lines Text UI.");
        return;
    }

    m_ScoreText->SetColor(Color{127, 85, 188, 255});
    m_ScoreText->Update("0");

    m_LevelText->SetColor(Color{127, 85, 188, 255});
    m_LevelText->Update("1");

    m_LinesText->SetColor(Color{127, 85, 188, 255});
    m_LinesText->Update("0");
}

auto GetBaseScoreByCompletedLines(const unsigned completedLines)
{
    if (completedLines == 1) { return 100; }
    else if (completedLines == 2) { return 200; }
    else if (completedLines == 3) { return 500; }
    else { return 800; }
}

auto UpdateText(std::shared_ptr<RenderText>& textObj, unsigned increment) 
{
    const auto prevRawText = textObj->GetRawText();
    const auto prevNumber = static_cast<unsigned>(std::stoi(prevRawText));
    const auto newNumber = prevNumber + increment;
    textObj->Update(std::to_string(newNumber));
    return newNumber;
}

auto StatsManager::UpdateScoreboard(const unsigned lines) -> void
{
    // Score
    auto score = GetBaseScoreByCompletedLines(lines);
    m_Score = UpdateText(m_ScoreText, score * m_Level);

    // Lines
    m_LinesCompleted = UpdateText(m_LinesText, lines);

    // Level
    if (m_LinesCompleted >= m_Level * 10)
    {
        m_Level++;
        m_LevelText->Update(std::to_string(m_Level));
    }
}

auto StatsManager::Draw() -> void
{
    const auto fontShader = ResourceManager::GetFromCache<Shader>({ResourceType::Shader, "FontShader"});

    m_ScoreText->Draw(fontShader.value(), (BlockSize * 14) - (m_ScoreText->GetSize().x / 2), (BlockSize * 5) - (m_ScoreText->GetSize().y / 2));
    m_LevelText->Draw(fontShader.value(), (BlockSize * 14) - (m_LevelText->GetSize().x / 2), ((BlockSize * 9) + 12) - (m_LevelText->GetSize().y / 2));
    m_LinesText->Draw(fontShader.value(), (BlockSize * 14) - (m_LinesText->GetSize().x / 2), ((BlockSize * 13) + 3) - (m_LinesText->GetSize().y / 2));
}