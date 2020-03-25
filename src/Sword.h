#ifndef SWORD_H
#define SWORD_H
#include "Weapon.h"
class Sword : public Weapon
{
  public:
    Sword();
    ~Sword();

    bool tick(float deltaTime) override;
    void start() override;

    friend class Weapon;
};
#endif // SWORD_H
