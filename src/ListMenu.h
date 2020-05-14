#ifndef LIST_MENU_H
#define LIST_MENU_H
#include "ListItem.h"
class ListMenu
{
  public:
    ListMenu();
    ~ListMenu();

    void paint();

    bool handleInput();

  private:
    IListItem** m_items;
    int         m_numItems;
    int         m_selectedIndex;
    int         m_prevSelectedIdx;
};
#endif // LIST_MENU_H
