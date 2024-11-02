#include <gui/MenuItem.h>
#include <RenderText.h>

#include <rendering/Shader.h>
#include <ResourceManager.h>

#include <Input.h>
#include <Window.h>
#include <gui/Menu.h>

MenuItemBase::MenuItemBase(Menu &menu, Window &window, const glm::vec2 &screenPos, const std::string &caption) : m_Menu{menu}, m_Window{window}, m_Position{screenPos}, m_Caption{caption}
{
    m_RenderText = std::make_shared<RenderText>("../../tetris/resources/fonts/tetris.ttf", 26);
    if (!m_RenderText)
    {
        SDL_Log("Unable to create text for menu item");
        return;
    }
}

void MenuItem::OnRender()
{
    const auto fontShader = ResourceManager::GetFromCache<Shader>({ResourceType::Shader, "FontShader"});

    m_RenderText->SetColor(GetTextColor());
    m_RenderText->Update(GetCaption());
    m_RenderText->Draw(fontShader.value(), m_Position.x - (m_RenderText->GetSize().x / 2), m_Position.y - (m_RenderText->GetSize().y / 2));
}

MenuItemSwitchOnOff::MenuItemSwitchOnOff(Menu &menu, Window &window, const glm::vec2 &screenPos,
                                         const std::string &caption,
                                         const Color &switchColor)
    : MenuItemBase{menu, window, screenPos, caption},
      m_SwitchColor(switchColor),
      m_Switch(false)
{
    m_SwitchText = std::make_shared<RenderText>("../../tetris/resources/fonts/tetris.ttf", 26);
    if (!m_SwitchText)
    {
        SDL_Log("Unable to create text for menu item");
        return;
    }
}

void MenuItemSwitchOnOff::OnRender()
{
    const auto fontShader = ResourceManager::GetFromCache<Shader>({ResourceType::Shader, "FontShader"});

    // Left text
    m_RenderText->SetColor(GetTextColor());
    m_RenderText->Update(GetCaption());
    m_RenderText->Draw(fontShader.value(), m_Position.x - (m_RenderText->GetSize().x / 2), m_Position.y - (m_RenderText->GetSize().y / 2));

    m_SwitchText->SetColor(m_SwitchColor);
    m_SwitchText->Update(GetSwitchState() ? "ON" : "OFF");
    m_SwitchText->Draw(fontShader.value(), (m_Position.x - (m_SwitchText->GetSize().x / 2)) + 230.0f, m_Position.y - (m_SwitchText->GetSize().y / 2));
}

void MenuItemSwitchOnOff::OnSelect()
{
    m_Switch = !m_Switch;
}

MenuItemList::MenuItemList(Menu &menu, Window &window, const glm::vec2 &screenPos, const std::string &caption, const Color &switchColor, const std::vector<std::string>& keys)
    : MenuItemSwitchOnOff{menu, window, screenPos, caption, switchColor}, m_Keys{keys}
{
}

void MenuItemList::OnRender()
{
    const auto fontShader = ResourceManager::GetFromCache<Shader>({ResourceType::Shader, "FontShader"});

    // Left text
    m_RenderText->SetColor(GetTextColor());
    m_RenderText->Update(GetCaption());
    m_RenderText->Draw(fontShader.value(), m_Position.x - (m_RenderText->GetSize().x / 2), m_Position.y - (m_RenderText->GetSize().y / 2));

    m_SwitchText->SetColor(m_SwitchColor);
    m_SwitchText->Update(GetCurrentKey());
    m_SwitchText->Draw(fontShader.value(), (m_Position.x - (m_SwitchText->GetSize().x / 2)) + 215.0f, m_Position.y - (m_SwitchText->GetSize().y / 2));
}

void MenuItemList::OnFrame() 
{
    if (Input::IsKeyPressed(m_Window, SDLK_RIGHT))
    {
        if (m_KeyIndex == (m_Keys.size() - 1)) 
        {
			m_KeyIndex = {};
			return;
		}
		m_KeyIndex++;
    }
    
    if (Input::IsKeyPressed(m_Window, SDLK_LEFT))
    {
        if (m_KeyIndex == 0) 
        {
			m_KeyIndex = (m_Keys.size() - 1);
			return;
		}
		m_KeyIndex--;
    }
}