#include "Technique.h"

Technique::Technique(int MPCost, const Sprite& sprite) :
    m_MPCost(MPCost), m_icon(sprite)
{
}

Technique::~Technique() {}

bool Technique::Tick(Adventurer&, float) { return false; }

bool Technique::HandleInput(Adventurer&) { return false; }

void Technique::Enter(Adventurer&) {}

void Technique::Exit(Adventurer&) {}
