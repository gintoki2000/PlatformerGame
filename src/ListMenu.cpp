#include "ListMenu.h"
#include "Game.h"
#include "Math.h"
#include "SDL_render.h"
#include "SpriteSheet.h"

ListMenu::ListMenu() : m_items(nullptr), m_numItems(0), m_selectedIndex(0) {}

ListMenu::~ListMenu()
{
    delete[] m_items;
    m_items = nullptr;
}

void ListMenu::paint()
{
    int numItemsPerPage = 4;
    int page            = m_prevSelectedIdx / numItemsPerPage;
    int beginIdx        = page * numItemsPerPage;
    int endIdx = Math::min(m_numItems - 1, beginIdx + numItemsPerPage - 1);

    int numItemsPerColumn = 2;
    int j                 = 0;
    int dstx              = 0;
    int dtsy              = 0;
    int vgap              = 5;
    int hgap              = 20;
    int sw                = 16;
    int sh                = 16;

    SDL_Rect      dstrect;
    SDL_Renderer* renderer = GAME->renderer();
    for (int i = beginIdx; i <= endIdx; ++i)
    {
        int r     = j / numItemsPerColumn;
        int c     = j % numItemsPerColumn;
        dstrect.x = dstx + c * (hgap * sw);
        dstrect.y = dtsy + r * (vgap * sh);
        dstrect.w = sw;
        dstrect.h = sh;

        const Sprite* icon = m_items[i]->getIcon();
        icon->draw(renderer, &dstrect);
        ++j;
    }
}
