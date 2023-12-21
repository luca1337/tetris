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

    auto UpdateScoreboard(const ScoreboardTextType, const unsigned newScore) -> void;
    auto Draw() -> void;

private:
    const Window& m_Window;

    std::shared_ptr<RenderText> m_ScoreText = {};
    std::shared_ptr<RenderText> m_LevelText = {};
    std::shared_ptr<RenderText> m_LinesText = {};
};