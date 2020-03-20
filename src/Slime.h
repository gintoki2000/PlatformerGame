#ifndef SLIME_H
#define SLIME_H
#include "Constances.h"
#include "Monster.h"
#include "SpriteSheet.h"
#include <memory>
class Animator;
class Slime : public Monster
{
  public:
    Slime(Level* level);
    ~Slime();

    void render(float deltaTime) override;
    void getHit(int damage) override;

	void tick(float deltaTime) override;
	void paint() override;

  private:
    void resetMembers();
    void updateGraphics(float deltaTime);
    void updatePhysics();
    void updateLogic(float deltaTime);
    void onPositionChanged() override;
	void setHorizontalSpeed(float speed);
	void stopHorizontalMovement();
	void attackPlayer();
	void synchronizeBodyTransform();
    enum
    {
        STATE_IDLE,
        STATE_WAIT,
        STATE_MOVE,
        STATE_ATTACK,
        STATE_HURT,
        STATE_DIE
    };
    enum
    {
        ANIM_IDLE,
        ANIM_MOVE,
        ANIM_ATTACK,
        ANIM_HURT,
        ANIM_DIE,
        NUM_ANIMS
    };

    int m_state;
    float m_timer;
    int m_direction;

    b2Body* m_body;
	std::shared_ptr<SpriteSheet> m_spriteSheet;
    Animator* m_animator;

    static constexpr float WIDTH = 32.f;
    static constexpr float HEIGHT = 25.f;
    static constexpr int SPRITE_WIDTH = 32;
    static constexpr int SPRITE_HEIGHT = 25;
    static constexpr float ACTIVATE_DISTANCE = Constances::TILE_SIZE * 8;
	static constexpr float ATTACK_DISTANCE = Constances::TILE_SIZE * 4;
	static constexpr float MOVE_SPEED = 10.f;
	static constexpr float MOVE_SPPED_2 = 15.f;

	static std::shared_ptr<SpriteSheet> sharedSpriteSheet;
};
#endif // SLIME_H
