#ifndef BASIC_ATTACK_H
#define BASIC_ATTACK_H
#include "Adventurer.h"
#include "Technique.h"
#include "Utils.h"
class BasicAttack : public Technique
{
  public:
    BasicAttack();

    void Enter(Adventurer& adventurer) override;
    void Exit(Adventurer& adventurer) override;
	bool HandleInput(Adventurer& adventurer);
    bool Tick(Adventurer& adventurer, float deltaTime) override;

  private:
	void DealDamage(Adventurer& adventurer, const Vec2& size);

    bool m_chain;
    int  m_phrase;
};
#endif // BASIC_ATTACK_H
