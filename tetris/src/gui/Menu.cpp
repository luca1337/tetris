#include <gui/Menu.h>
#include <gui/MenuPage.h>
#include <gui/MenuItem.h>

#include <Window.h>
#include <Logger.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct MenuItemPlay : public MenuItem
{
public:
	MenuItemPlay(Menu& menu, Window& window, const glm::vec2& screenPos, const std::string& caption)
		: MenuItem(menu, window, screenPos, caption)
	{}

	virtual void Execute() override
	{
        m_Window.IsPaused() = false;
	}
};

struct MenuItemSettings : public MenuItem
{
public:
    MenuItemSettings(Menu& menu, Window& window, const glm::vec2& screenPos, const std::string& caption)
		: MenuItem(menu, window, screenPos, caption)
	{}

    virtual void Execute() override
    {
        m_Menu.SetCurrentPage(MenuPageType::Settings);
    }
};

struct MenuItemSwitchPostProcessing : public MenuItemSwitchOnOff
{
public:
    MenuItemSwitchPostProcessing(Menu& menu, Window& window, const glm::vec2& screenPos, const std::string& caption)
        : MenuItemSwitchOnOff{menu, window, screenPos, caption, Cyan}
    {
        m_Switch = m_Window.IsPostProcessingEnabled();
    }

    virtual void Execute() override
    {
        m_Window.SetPostProcessingEnabled(GetSwitchState());
    }
};

struct MenuItemGoBack : public MenuItem
{
public:
    MenuItemGoBack(Menu& menu, Window& window, const glm::vec2& screenPos, const std::string& caption, const MenuPageType toPage)
        : MenuItem{menu, window, screenPos, caption}, m_ToPage{toPage}
    {
    }

    virtual void Execute() override
    {
        m_Menu.SetCurrentPage(m_ToPage);
    }
private:
    MenuPageType m_ToPage = {};
};

struct MenuItemSelectPP : public MenuItemList
{
public:
    MenuItemSelectPP(Menu& menu, Window& window, const glm::vec2& screenPos, const std::string& caption, const std::vector<std::string>& keys)
        : MenuItemList{menu, window, screenPos, caption, Cyan, keys}
    {
    }

    virtual void Execute() override
    {
        const auto KeyToEnum = [](const auto& key) {
            if (key == "CCTV") { return PostProcessingType::cctv;
            } else if (key == "CHROMA") { return PostProcessingType::chroma;
            } else if (key == "EDGE-DET") { return PostProcessingType::edge_detection;
            } else if (key == "EMBOSS") { return PostProcessingType::embossing; }
            else { return PostProcessingType::cctv; }
        };

        m_Window.SetPostProcessingByType(KeyToEnum(GetCurrentKey()));
    }
};

auto CalculatePosition(const Window& window, float xOffset, float yOffset) 
{
    return glm::vec2{(window.Props().width / 2) + xOffset, (window.Props().height / 2) + yOffset};
}

Menu::Menu(Window &window) : m_Window{window}
{
    m_ItemTitle = std::make_shared<MenuItem>(*this, window, CalculatePosition(window, -100.0f, -60.0f), "- PAUSE -");

    const auto mainPage = std::make_shared<MenuPage>(window, "Main");

    const auto playItem = std::make_shared<MenuItemPlay>(*this, window, CalculatePosition(window, -200.0f, 0.0f), "> PLAY");
    const auto settingsItem = std::make_shared<MenuItemSettings>(*this, window, CalculatePosition(window, -175.0f, 40.0f), "> SETTINGS");

    mainPage->AddItem(playItem);
    mainPage->AddItem(settingsItem);

    m_Pages.emplace(MenuPageType::Main, mainPage);

    const auto settingsPage = std::make_shared<MenuPage>(window, "Settings");

    const auto postProcessingItem = std::make_shared<MenuItemSwitchPostProcessing>(*this, window, CalculatePosition(window, -128.0f, 0.0f), "> POST PROCESSING");
    const auto selectPostProcessingItem = std::make_shared<MenuItemSelectPP>(*this, window, CalculatePosition(window, -98.0f, 40.0f), "> POST PROCESSING TYPE", std::vector<std::string>{"CCTV", "CHROMA", "EDGE-DET", "EMBOSS"});
    const auto goBackItem = std::make_shared<MenuItemGoBack>(*this, window, CalculatePosition(window, -181.0f, 80.0f), "> GO BACK", MenuPageType::Main);

    settingsPage->AddItem(postProcessingItem);
    settingsPage->AddItem(selectPostProcessingItem);
    settingsPage->AddItem(goBackItem);

    m_Pages.emplace(MenuPageType::Settings, settingsPage);
}

auto Menu::Render() -> void 
{
    if (!m_IsVisible) { return; }

    m_CurrentPage->OnRender();
    m_CurrentPage->OnUpdate();

    m_ItemTitle->OnRender();
}

auto Menu::Show(bool value) -> void 
{
    m_IsVisible = value;

    if (m_IsVisible)
    {
        m_CurrentPage = m_Pages[MenuPageType::Main];
    }
}