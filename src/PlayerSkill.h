#ifndef PLAYER_SKILL_H
#define PLAYER_SKILL_H
class Player;
struct PlayerSkill
{
		virtual ~PlayerSkill();	

        virtual bool activate(Player& player) = 0;
        virtual void enter(Player& player);
		virtual bool tick(Player& player, float deltaTime) = 0;
        virtual void exit(Player& player);
};

#endif //PLAYER_SKILL_H
