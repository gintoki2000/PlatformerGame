#ifndef PLAYER_H
#define PLAYER_H
#include "NTLayer.h"
#include "Box2D/Box2D.h"
#include "Animation.h"
#include "NTTextureRegion.h"
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
    static constexpr float WIDTH = 50.f;
    static constexpr float HEIGHT = 37.f;
	static constexpr int SPRITE_WIDTH = 50;
	static constexpr int SPRITE_HEIGHT = 37;

  public:
    static Player* create(Level* level);

    ~Player() override;

    void update(float dt) override;

    void draw(SDL_Renderer* renderer, const NTRect& viewPort) override;

  private:
    Player();

    bool initialize(Level* level);

    bool m_isOnGround;

    AnimationState m_animationState;

	float m_animationTimeState;

    b2Body* m_body;

    Level* m_level;

	Animation<NTTextureRegion> m_animations[NUM_ANIMATIONS];		

	SDL_Texture* m_texture;
};
#endif // PLAYER_H
