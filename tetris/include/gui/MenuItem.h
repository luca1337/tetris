#pragma once

#include <string>
#include <memory>
#include <vector>
#include <functional>

#include <glm/vec2.hpp>

#include <Color.h>

class RenderText;
class Window;
class Menu;
struct SDL_Renderer;

const Color selectedItemColor = { 255, 0, 0, 255 };

class MenuItemBase
{
public:
	MenuItemBase(Menu& menu, Window& window, const glm::vec2& screenPos, const std::string& caption);
	virtual						~MenuItemBase() { }
	virtual void				OnRender() { }
	virtual void				Execute() { }
	virtual void				OnSelect() { }
	virtual void 				OnFrame() { }
	std::string					GetCaption() const { return m_Caption; }

	const Color&				GetTextColor() const { return !m_State ? m_TextColor : selectedItemColor; }
	void						SetState(bool value) { m_State = value; }
	bool						GetState() { return m_State; }
protected:
	Menu& 						m_Menu;
	Window& 					m_Window;
	glm::vec2 					m_Position = {};
	std::string 				m_Caption = {};
	const Color 				m_TextColor { 255, 255, 255, 255 };
	bool			 			m_State = {};
    std::shared_ptr<RenderText> m_RenderText = {};
};

/// <summary>
/// MENU ITEM DEFAULT
/// </summary>
class MenuItem : public MenuItemBase
{
public:
	MenuItem(Menu& menu, Window& window, const glm::vec2& screenPos, const std::string& caption)
        : MenuItemBase(menu, window, screenPos, caption)
    {}

	virtual void				OnRender() override;
};

/// <summary>
/// MENU ITEM SWITCH ON/OFF
/// </summary>
class MenuItemSwitchOnOff : public MenuItemBase
{
public:
	MenuItemSwitchOnOff(Menu& menu, Window &window, const glm::vec2 &screenPos,
                                         const std::string &caption,
                                         const Color &switchColor);
	virtual void				OnRender() override;
	virtual void				OnSelect() override;
	bool						GetSwitchState() const { return m_Switch; }
protected:
	std::shared_ptr<RenderText> m_SwitchText = {};
	Color m_SwitchColor;
	bool m_Switch;
};

/// <summary>
/// MENU ITEM LIST 
/// </summary>
class MenuItemList : public MenuItemSwitchOnOff
{
public:
	MenuItemList(Menu& menu, Window &window, const glm::vec2 &screenPos,
                                         const std::string &caption,
                                         const Color &switchColor,
										 const std::vector<std::string>& keys);
	virtual void				OnRender() override;
	virtual void				OnFrame() override;

	std::string					GetCurrentKey() const { return m_Keys[m_KeyIndex]; }
private:
	std::vector<std::string> m_Keys = {};
	unsigned m_KeyIndex = {};
};