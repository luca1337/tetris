#pragma once

#include <functional>

class Timer
{
public:
    ~Timer() = default;
    Timer() = default;
    explicit Timer(const float maxTime);
    
    auto Update(const float deltaTime, const std::function<void(float)>& onUpdateFunc, const std::function<void(float)>& onCompletedFunc) -> void;
    auto SetMaxTime(const float maxTime) { m_MaxTime = std::max(0.0f, maxTime); }
    auto Reset() { m_ElapsedTime = 0.0f; }
    
    [[nodiscard]] auto IsRunning() const { return m_ElapsedTime <= m_MaxTime; }
    [[nodiscard]] auto GetRemainingTime() const { return std::max(0.0f, m_MaxTime - m_ElapsedTime); }

private:
    float m_ElapsedTime = {};
    float m_MaxTime = {};
};