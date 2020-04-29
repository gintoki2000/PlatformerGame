#ifndef SLASH_H
#define SLASH_H
#include "PlayerSkill.h"
class Slash : public PlayerSkill
{
  public:
	static Slash* create();
    bool activate(Player& player) override;
    void enter(Player& player) override;
    bool tick(Player& player, float deltaTime) override;
    void exit(Player& player) override;
private:
    Slash();
	bool init();

	
};
#endif // SLASH_H
