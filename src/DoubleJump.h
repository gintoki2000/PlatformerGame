#ifndef DOUBLE_JUMP_H
#define DOUBLE_JUMP_H
#include "Ability.h"
class DoubleJump : public Ability
{
  public:
    DoubleJump() {}

    void enable(Adventurer& adventurer) override;
    void disable(Adventurer& adventurer) override;
};
#endif // DOUBLE_JUMP_H
