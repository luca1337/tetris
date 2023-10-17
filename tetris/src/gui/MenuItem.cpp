#include <gui/MenuItem.h>
#include <RenderText.h>

#include <SDL2/SDL.h>

MenuItem::MenuItem(SDL_Renderer& renderer, const glm::vec2& screenPos, const std::string &caption, const std::function<void()>& onSelect) 
    : m_SdlRenderer{renderer}, m_Position{screenPos}, m_Text{caption}, m_OnSelect{onSelect}
{
    m_RenderText = std::make_shared<RenderText>(&renderer, "../../tetris/resources/fonts/tetris.ttf", 32);
    if (!m_RenderText)
    {
        SDL_Log("Unable to create text for menu item");
        return;
    }
}

auto MenuItem::OnRender() -> void
{
    const auto textColor = m_IsSelected ? Color{0xff, 0xff, 0xff, 0xff} : Color{0xff, 0x0, 0x0, 0xff};

    m_RenderText->SetColor(textColor);
    m_RenderText->Update(m_Text);
    m_RenderText->Draw(m_Position.x, m_Position.y);
}

auto MenuItem::OnSelect() -> void
{
    if (m_OnSelect)
    {
        m_OnSelect();
    }
}