#include "Inventoty.h"
#include "Item.h"
#include "SDL.h"
#include "Technique.h"

Inventory::Inventory()
{
    m_items.reserve(10);
    m_techinques.reserve(10);
}

Inventory::~Inventory()
{
    for (auto& item : m_items)
    {
        delete item;
    }

	for (auto& tech : m_techinques)
	{
		delete tech;
	}

	m_items.clear();
	m_techinques.clear();
}


void Inventory::AddTechnique(Technique* technique)
{
    m_techinques.push_back(technique);
}

Technique* Inventory::GetTechnique(int i)
{
    return m_techinques[static_cast<unsigned>(i)];
}

int Inventory::GetNumTechniques() const { return m_techinques.size(); }

void Inventory::AddItem(Item* item) { m_items.push_back(item); }

int Inventory::GetNumItems() const { return m_items.size(); }

Item* Inventory::GetItem(int i) { return m_items[static_cast<unsigned>(i)]; }
