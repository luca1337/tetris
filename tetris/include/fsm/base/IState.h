#pragma once

#include <memory>

class Window;

class IState : public std::enable_shared_from_this<IState>
{
public:
    virtual ~IState() = default;
    virtual auto OnStateEnter() -> void = 0;
    virtual auto OnStateUpdate() -> std::shared_ptr<IState> = 0;
    virtual auto OnStateExit() -> void = 0;
};