#ifndef PLAYER_TEST_SKILL_H
#define PLAYER_TEST_SKILL_H
#include "PlayerSkill.h"
class PlayerDashSkill : public PlayerSkill
{
  public:
    PlayerDashSkill();

    bool activate(Player& player) override;
    bool tick(Player& player, float deltaTime) override;

  private:
    float m_time;
    int   m_prevAnimation;
    float m_prevTimeState;
    int   m_leftTimes;
    int   m_maxDashTimes;
};
#endif // PLAYER_TEST_SKILL_H
