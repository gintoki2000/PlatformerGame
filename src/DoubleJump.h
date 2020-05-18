#ifndef DOUBLE_JUMP_H
#define DOUBLE_JUMP_H
#include "Ability.h"
class DoubleJump : public Ability
{
  public:
    DoubleJump() {}

    void Enable(Adventurer& adventurer) override;
    void Disable(Adventurer& adventurer) override;
};
#endif // DOUBLE_JUMP_H
