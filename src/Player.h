#ifndef PLAYER_H
#define PLAYER_H
#include "NTLayer.h"
#include <Box2D/Box2D.h>
class Level;
class Player : public NTLayer
{
  private:
    enum AnimationState
    {
        ANIMATION_IDLE_1,
        ANIMATION_IDLE_2,
        ANIMATION_RUN,
        ANIMATION_JUMP,
        ANIMATION_GRAB,
        ANIMATION_CLIMB,
        ANIMATION_HURT,
        ANIMATION_FALL,
        ANIMATION_ATTACK_1,
        ANIMATION_ATTACK_2,
        ANIMATION_ATTACK_3,
		ANIMATION_AIR_ATTACK_1,
		ANIMATION_AIR_ATTACK_2,
		ANIMATION_AIR_ATTACK_3,
		NUM_ANIMATIONS
    };

  public:

	Player* create(Level* level);

	~Player() override;

	void update(float dt) override;

	void draw(SDL_Renderer* renderer, const NTRect& viewPort) override;

  private:
    Player();

	bool initialize(Level* level);

	bool m_isOnGround;

	AnimationState m_animationState;	

	b2Body* m_body;

	Level* m_level;
};
#endif // PLAYER_H
