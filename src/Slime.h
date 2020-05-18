#ifndef SLIME_H
#define SLIME_H
/*#include "Constances.h"
#include "Monster.h"
#include "SpriteSheet.h"
class Animator;
class Slime : public Monster
{
  public:
	Slime(Level* level);
    ~Slime();

    void GetHit(int damage) override;

  private:
    Slime();
	void UpdateLogic(float deltaTime) override;
    bool Init(Level*);
    void ResetMembers();
    void CheckAttackCollision();
    void CheckDirection();

    void Idle();
    void Wait();
    void Move();
    void Attack();
    void Hurt();
    void Die();
    enum
    {
        ANIM_IDLE,
        ANIM_MOVE,
        ANIM_ATTACK,
        ANIM_HURT,
        ANIM_DIE,
        NUM_ANIMS
    };

    enum State
    {
        STATE_IDLE,
        STATE_WAIT,
        STATE_MOVE,
        STATE_ATTACK,
        STATE_HURT,
        STATE_DIE
    };

    void         SetState(State newState, float initialTime);
    State        m_state;
    float        m_timer;
    float        m_changingDirTimer;
    SpriteSheet* m_spriteSheet;

    static constexpr float WIDTH             = 32.f;
    static constexpr float HEIGHT            = 25.f;
    static constexpr int   SPRITE_WIDTH      = 32;
    static constexpr int   SPRITE_HEIGHT     = 25;
    static constexpr float ACTIVATE_DISTANCE = Constances::TILE_SIZE * 5;
    static constexpr float ATTACK_DISTANCE   = Constances::TILE_SIZE * 1;
    static constexpr float MOVE_SPEED        = 1.f;
    static constexpr float MOVE_SPPED_2      = 15.f;
};*/
#endif // SLIME_H
