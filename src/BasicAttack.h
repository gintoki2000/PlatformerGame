#ifndef BASIC_ATTACK_H
#define BASIC_ATTACK_H
#include "Technique.h"
#include "Utils.h"
class BasicAttack : public Technique
{
  public:
    BasicAttack();
    void Enter(Adventurer& adventurer) override;
    void Exit(Adventurer& adventurer) override;
    bool Tick(Adventurer& adventurer, float deltaTime) override;

  private:
    void Check(const Vec2& position, const Vec2& size, Direction direction);

    bool m_chain;
    int  m_phrase;
};
#endif // BASIC_ATTACK_H
