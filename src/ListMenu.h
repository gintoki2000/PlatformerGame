#ifndef LIST_MENU_H
#define LIST_MENU_H
#include "ListItem.h"
class EquipmentMenu;

class ListMenu
{
  public:
    ListMenu(EquipmentMenu* equipmentMenu);
    ~ListMenu();

    void Paint();
    void HandleInput();
    void SetItems(IListItem** items, int numItems);

  private:
    void Clear();

    EquipmentMenu* m_equipmentMenu;
    IListItem**    m_items;
    int            m_numItems;
    int            m_selectedIndex;
    int            m_numItemsPerPage;
    int            m_numItemsPerColumn;
};
#endif // LIST_MENU_H
