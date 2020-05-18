#ifndef LIST_MENU_H
#define LIST_MENU_H
#include "ListItem.h"
#include <vector>
class EquipmentMenu;

class ListMenu
{
  public:
    ListMenu(EquipmentMenu* equipmentMenu);
    ~ListMenu();

    void Paint();
    void HandleInput();
    void SetItems(const std::vector<IListItem*>& items);
    int  GetNumItems() const;

  private:
    void Clear();

    EquipmentMenu*          m_equipmentMenu;
    std::vector<IListItem*> m_items;
    int                     m_selectedIndex;
    int                     m_numItemsPerPage;
    int                     m_numItemsPerColumn;
};
#endif // LIST_MENU_H
