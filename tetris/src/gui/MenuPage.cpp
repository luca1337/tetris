#include <gui/MenuPage.h>
#include <gui/MenuItem.h>
#include <Input.h>
#include <Window.h>

MenuPage::MenuPage(const Window& window, const std::string& title) : m_Window{window}, m_PageItems{}
{
}

auto MenuPage::AddItem(const std::shared_ptr<MenuItemBase> &item) -> void
{
    m_PageItems.push_back(item); 
    m_CurrentItem = m_PageItems[0]; 
    m_CurrentItem->SetState(true);
}

auto MenuPage::OnRender() -> void 
{
    for (auto&& menuItem: m_PageItems)
    {
        menuItem->OnRender();
    }
}

auto MenuPage::OnUpdate() -> void 
{
    if (Input::IsKeyPressed(m_Window, SDLK_DOWN)) 
    {
        m_PageItems[m_ItemIndex]->SetState(false);

        m_ItemIndex++;

        if (m_ItemIndex >= m_PageItems.size()) 
        {
            m_ItemIndex = 0;
        }

        m_PageItems[m_ItemIndex]->SetState(true);
        m_CurrentItem = m_PageItems[m_ItemIndex];
    }

    if (Input::IsKeyPressed(m_Window, SDLK_UP)) 
    {
        m_PageItems[m_ItemIndex]->SetState(false);

        m_ItemIndex--;

        if (m_ItemIndex < 0) 
        {
            m_ItemIndex = m_PageItems.size() - 1;
        }

        m_PageItems[m_ItemIndex]->SetState(true);
        m_CurrentItem = m_PageItems[m_ItemIndex];
    }

    if (Input::IsKeyPressed(m_Window, SDLK_SPACE))
    {
        m_CurrentItem->OnSelect();
        m_CurrentItem->Execute();
    }

    m_CurrentItem->OnFrame();
}
