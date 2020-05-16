#include "Inventoty.h"
#include "SDL.h"
#include "Technique.h"

Inventory::Inventory() : m_numTechniques(0) {}

Inventory::~Inventory()
{
    for (int i = 0; i < m_numTechniques; ++i)
    {
        delete m_techinques[i];
        m_techinques[i] = nullptr;
    }
}

void Inventory::AddTechnique(Technique* technique)
{
    SDL_assert(m_numTechniques < MAX_TECHNIQUES);
    m_techinques[m_numTechniques++] = technique;
}

Technique* Inventory::GetTechnique(int i) { return m_techinques[i]; }

int Inventory::GetNumTechniques() const { return m_numTechniques; }
