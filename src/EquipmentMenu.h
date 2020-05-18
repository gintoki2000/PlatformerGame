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
    void Tick(float deltaTime) override;
    void Paint() override;
    void HandleInput();
    void OnBecomeVisible() override;

    void OnItemSelected(int index);

  private:
    enum State
    {
        STATE_MAIN,
        STATE_SUB_TECNIQUE,
        STATE_MAIN_TECNIQUE,
        STATE_ITEM
    };

    void UpdateList(State state);


    ListMenu*   m_listMenu;
    Adventurer* m_adventurer;
    State       m_state;
};
#endif // PAUSE_MENU_H
