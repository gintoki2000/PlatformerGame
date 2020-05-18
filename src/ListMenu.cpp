#include "ListMenu.h"
#include "EquipmentMenu.h"
#include "Game.h"
#include "Input.h"
#include "ListItem.h"
#include "Math.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include "SpriteSheet.h"

ListMenu::ListMenu(EquipmentMenu* equipmentMenu) :
    m_equipmentMenu(equipmentMenu), m_selectedIndex(0), m_numItemsPerPage(4),
    m_numItemsPerColumn(2)
{
}

ListMenu::~ListMenu() { Clear(); }

void ListMenu::Paint()
{
    if (GetNumItems() == 0)
        return;
    int page     = m_selectedIndex / m_numItemsPerPage;
    int beginIdx = page * m_numItemsPerPage;
    int endIdx   = Math::Min(GetNumItems() - 1, beginIdx + m_numItemsPerPage - 1);

    int j    = 0;
    int dstx = 100;
    int dtsy = 50;
    int vgap = 5;
    int hgap = 20;
    int sw   = 16;
    int sh   = 16;

    SDL_Rect      dstrect;
    SDL_Renderer* renderer = GAME->GetRenderer();
    for (int i = beginIdx; i <= endIdx; ++i, ++j)
    {
        int r     = j / m_numItemsPerColumn;
        int c     = j % m_numItemsPerColumn;
        dstrect.x = dstx + c * (hgap + sw);
        dstrect.y = dtsy + r * (vgap + sh);
        dstrect.w = sw;
        dstrect.h = sh;

        const Sprite* icon = m_items[i]->GetIcon();
        icon->Draw(renderer, &dstrect);

        if (i == m_selectedIndex)
        {
            SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
            SDL_RenderDrawRect(renderer, &dstrect);
        }
    }
}

void ListMenu::SetItems(const std::vector<IListItem*>& items)
{
    m_items = items;
}

int ListMenu::GetNumItems() const { return m_items.size(); }

void ListMenu::HandleInput()
{
    if (Input::IsJustPressed(BUTTON_A))
    {
        m_equipmentMenu->OnItemSelected(m_selectedIndex);
    }
    else if (Input::IsJustPressed(BUTTON_PAD_LEFT))
    {
        m_selectedIndex -= 1;
    }
    else if (Input::IsJustPressed(BUTTON_PAD_RIGHT))
    {
        m_selectedIndex += 1;
    }
    else if (Input::IsJustPressed(BUTTON_PAD_UP))
    {
        m_selectedIndex -= m_numItemsPerColumn;
    }
    else if (Input::IsJustPressed(BUTTON_PAD_DOWN))
    {
        m_selectedIndex += m_numItemsPerColumn;
    }

    if (m_selectedIndex < 0)
    {
        m_selectedIndex = 0;
    }
    if (m_selectedIndex > GetNumItems() - 1)
    {
        m_selectedIndex = GetNumItems() - 1;
    }
}

void ListMenu::Clear()
{
	
}
