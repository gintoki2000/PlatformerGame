#ifndef ABILITY_H
#define ABILITY_H
class Adventurer;
class Ability
{
  public:
    virtual ~Ability() {}

    virtual void Enable(Adventurer& adventurer)  = 0;
    virtual void Disable(Adventurer& adventurer) = 0;
};
#endif // BILITY_H
