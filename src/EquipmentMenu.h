#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H
#include "Adventurer.h"
#include "Layer.h"
#include "ListMenu.h"
class EquipmentMenu : public Layer
{
  public:
    EquipmentMenu();
    ~EquipmentMenu();

    void Start() override;
    void Update(float deltaTime) override;
    void Render() override;
    void HandleInput();
    void OnBecomeVisible() override;

    void OnItemSelected(int index);

  private:
    void UpdateList();

    ListMenu*   m_listMenu;
    Adventurer* m_adventurer;
};
#endif // PAUSE_MENU_H
