#include <Timer.h>

Timer::Timer(const float maxTime) : m_MaxTime{maxTime} { }

auto Timer::Update(const float deltaTime, const std::function<void(float)>& onUpdateFunc, const std::function<void(float)> &onCompletedFunc) -> void
{
    m_ElapsedTime += deltaTime;

    if (m_ElapsedTime >= m_MaxTime)
    {
        onCompletedFunc(m_ElapsedTime);
    }
    else
    {
        if (onUpdateFunc)
            onUpdateFunc(m_ElapsedTime);
    }
}