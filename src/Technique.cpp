#include "Technique.h"

Technique::Technique(int MPCost, const Sprite& sprite) :
    m_MPCost(MPCost), m_icon(sprite)
{
}

Technique::~Technique() {}

void Technique::enter(Adventurer&) {}

void Technique::exit(Adventurer&) {}
