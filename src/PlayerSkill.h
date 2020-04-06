#ifndef PLAYER_SKILL_H
#define PLAYER_SKILL_H
class Player;
class PlayerSkill
{
	public:
		virtual ~PlayerSkill();	

        virtual bool activate(Player& player) = 0;
		virtual bool tick(Player& player, float deltaTime) = 0;
};
#endif //PLAYER_SKILL_H
