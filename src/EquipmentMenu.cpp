#include "EquipmentMenu.h"
#include "Constances.h"
#include "Game.h"
#include "Input.h"
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

void EquipmentMenu::Update(float)
{
    if (Input::IsJustPressed(BUTTON_B))
    {
        Level* level = static_cast<Level*>(GetScene());
        level->HideEquipmentMenu();
    }
}
void EquipmentMenu::Render() { m_listMenu->Paint(); }

void EquipmentMenu::HandleInput() { m_listMenu->HandleInput(); }

void EquipmentMenu::OnBecomeVisible() { UpdateList(); }

void EquipmentMenu::OnItemSelected(int index)
{
    Technique* technique = m_adventurer->GetInventory()->GetTechnique(index);
    m_adventurer->SetTechniqueA(technique);

    Level* level = static_cast<Level*>(GetScene());
    level->HideEquipmentMenu();
}

void EquipmentMenu::UpdateList()
{
    int        numItems = m_adventurer->GetInventory()->GetNumTechniques();
    IListItem* items[numItems];
    for (int i = 0; i < numItems; ++i)
    {
        items[i] = m_adventurer->GetInventory()->GetTechnique(i);
    }
    m_listMenu->SetItems(items, numItems);
}
