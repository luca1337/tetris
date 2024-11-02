#pragma once

#include <memory>
#include <unordered_map>

class Window;
class MenuPage;
class MenuItem;

enum class MenuPageType { Main, Settings };

class Menu
{
public:
    Menu(Window &window);
    auto Render() -> void;
    auto Show(bool value) -> void;
    auto SetCurrentPage(const MenuPageType pageType) { m_CurrentPageType = pageType; m_CurrentPage = m_Pages[m_CurrentPageType]; }
private:
    Window& m_Window;
    std::unordered_map<MenuPageType, std::shared_ptr<MenuPage>> m_Pages = {};
    std::shared_ptr<MenuPage> m_CurrentPage = {};
    std::shared_ptr<MenuItem> m_ItemTitle = {};
    MenuPageType m_CurrentPageType = MenuPageType::Main;

    bool m_IsVisible = {};
};