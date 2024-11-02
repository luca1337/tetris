#pragma once

#include <memory>

class RenderText;
class Window;

enum class ScoreboardTextType{
    Score,
    Level,
    Lines
};

class StatsManager
{
public:
    ~StatsManager() = default;
    StatsManager(const Window&);
    StatsManager(const StatsManager&) = delete;
    StatsManager(StatsManager&&) = delete;

    auto UpdateScoreboard(const unsigned lines) -> void;
    auto Draw() -> void;

    auto GetLevel() const { return m_Level; }

private:
    const Window& m_Window;

    std::shared_ptr<RenderText> m_ScoreText = {};
    std::shared_ptr<RenderText> m_LevelText = {};
    std::shared_ptr<RenderText> m_LinesText = {};

    unsigned m_Score= {};
    unsigned m_Level= 1;
    unsigned m_LinesCompleted= {};
};