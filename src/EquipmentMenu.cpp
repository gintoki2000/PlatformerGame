#include "EquipmentMenu.h"
#include "Constances.h"
#include "Game.h"
#include "Input.h"
#include "Item.h"
#include "Level.h"
#include "ListItem.h"
#include "ListMenu.h"
#include "SDL_render.h"
#include "Scene.h"
#include "Technique.h"
#include "Utils.h"
EquipmentMenu::EquipmentMenu() { m_listMenu = new ListMenu(this); }

EquipmentMenu::~EquipmentMenu() { DELETE_NULL(m_listMenu); }

void EquipmentMenu::Start()
{
    m_adventurer = static_cast<Level*>(GetScene())->GetAdventurer();
}

void EquipmentMenu::Tick(float)
{
    if (Input::IsJustPressed(BUTTON_B))
    {
        Level* level = static_cast<Level*>(GetScene());
        level->HideEquipmentMenu();
    }
}
void EquipmentMenu::Paint() { m_listMenu->Paint(); }

void EquipmentMenu::HandleInput() { m_listMenu->HandleInput(); }

void EquipmentMenu::OnBecomeVisible() { UpdateList(STATE_SUB_TECNIQUE); }

void EquipmentMenu::OnItemSelected(int index)
{
    Technique* technique = m_adventurer->GetInventory()->GetTechnique(index);
    m_adventurer->SetTechniqueA(technique);

    Level* level = static_cast<Level*>(GetScene());
    level->HideEquipmentMenu();
}

void EquipmentMenu::UpdateList(State state)
{
    std::vector<IListItem*> items;
    switch (state)
    {
    case STATE_SUB_TECNIQUE:
    {
        int numItems = m_adventurer->GetInventory()->GetNumTechniques();
        items.reserve (numItems);
        for (int i = 0; i < numItems; ++i)
        {
            items[i] = m_adventurer->GetInventory()->GetTechnique(i);
        }
        m_listMenu->SetItems(items);
    }
    case STATE_ITEM:
    {
        int numItems = m_adventurer->GetInventory()->GetNumItems();
        items.reserve(numItems);
        for (int i = 0; i < numItems; ++i)
        {
            items[i] = m_adventurer->GetInventory()->GetItem(i);
        }
        m_listMenu->SetItems(items);
    }
    }
}
