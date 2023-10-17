#pragma once

#include <string>
#include <memory>
#include <functional>

#include <glm/vec2.hpp>

class RenderText;
struct SDL_Renderer;

class MenuItem
{
public:
    MenuItem(SDL_Renderer& renderer, const glm::vec2& screenPos, const std::string& caption, const std::function<void()>& onSelect);
    auto SetState(const bool state) { m_IsSelected = state; }
    auto OnSelect() -> void;
    auto OnRender() -> void;
private:
    std::shared_ptr<RenderText> m_RenderText = {};
    bool m_IsSelected = {};

private:
    SDL_Renderer& m_SdlRenderer;
    std::function<void()> m_OnSelect = {};
    std::string m_Text = {};
    glm::vec2 m_Position = {};
};