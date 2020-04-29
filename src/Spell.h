#ifndef SPELL_H
#define SPELL_H
#include "GameObject.h"
class Spell : public GameObject
{
  public:
    Spell() : m_manaCost(1) {}

    int  getManaCost() const { return m_manaCost; }
    void setManaCost(int manaCost) { m_manaCost = manaCost; }

  private:
    int m_manaCost;
};
#endif // SPELL_H
