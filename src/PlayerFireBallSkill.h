#ifndef PLAYER_FIREBALL_SKILL_H
#define PLAYER_FIREBALL_SKILL_H
#include "PlayerSkill.h"
class PlayerFireballSkill : public PlayerSkill
{
  public:
    PlayerFireballSkill();

    bool activate(Player& player) override;
    bool tick(Player& player, float deltaTime) override;

  private:
    int   m_phrase;
    float m_timer;
};
#endif // PLAYER_FIREBALL_SKILL_H
