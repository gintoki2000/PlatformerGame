#ifndef PLAYER_CUT_SKILL_H
#define PLAYER_CUT_SKILL_H
#include "PlayerSkill.h"
class PlayerCutSkill : public PlayerSkill
{
  public:
    PlayerCutSkill();

    bool activate(Player& player) override;
    bool tick(Player& player, float deltaTime) override;

  private:
    int   m_prevAnimation;
    float m_prevTimeState;
};
#endif // PLAYER_CUT_SKILL_H
