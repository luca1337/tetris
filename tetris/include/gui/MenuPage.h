#pragma once

#include <string>
#include <memory>
#include <functional>

#include <glm/vec2.hpp>

class MenuItemBase;
class Window;

class MenuPage
{
public:
    MenuPage(const Window& window, const std::string& title);
    auto AddItem(const std::shared_ptr<MenuItemBase>& item) -> void;

    auto OnRender() -> void;
    virtual auto OnUpdate() -> void;

private:
    const Window& m_Window;
    std::vector<std::shared_ptr<MenuItemBase>> m_PageItems = {};
    std::shared_ptr<MenuItemBase> m_CurrentItem = {};
    int m_ItemIndex = {};
};