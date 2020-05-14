#ifndef ABILITY_H
#define ABILITY_H
class Adventurer;
class Ability
{
  public:
    virtual ~Ability() {}

    virtual void enable(Adventurer& adventurer)  = 0;
    virtual void disable(Adventurer& adventurer) = 0;
};
#endif // BILITY_H
