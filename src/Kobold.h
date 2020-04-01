#ifndef KOBLOD_H
#define KOBLOD_H
#include "Animator.h"
#include "Enums.h"
#include "Monster.h"
#include "SpriteSheet.h"
#include "box2d/box2d.h"
class Koblod : public Monster
{
    enum State
    {
        STATE_IDLE,
        STATE_RUN,
        STATE_ATTACK,
        STATE_HURT,
        STATE_DIE,
        STATE_WAIT
    };

    enum Anim
    {
        ANIM_IDLE,
        ANIM_RUN,
        ANIM_ATTACK,
        ANIM_HURT,
        ANIM_DIE,
        NUM_ANIMS
    };

  public:
    Koblod(Level* level);
    ~Koblod();
	void getHit(int damage) override;

  private:
    void updateLogic(float deltaTime) override;
    void resetMembers();

    void idle();
    void run();
    void attack();
    void wait();
    void hurt();
    void die();

    State     m_state;
    float     m_timer;

    SpriteSheet* m_spriteSheet;

  public:
    static constexpr int SPRITE_WIDTH     = 86;
    static constexpr int SPRITE_HEIGHT    = 69;
    static constexpr int WIDTH            = 21;
    static constexpr int HEIGHT           = 30;
    static constexpr int ACTIVATE_RUN_DIS = 4 * 16;
    static constexpr int ACTIVATE_ATK_DIS = 2 * 16;
};
#endif // KOBLOD_H
