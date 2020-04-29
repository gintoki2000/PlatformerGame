#ifndef BOARWARRIOR_H
#define BOARWARRIOR_H
#include "Animator.h"
#include "Monster.h"
#include "SDL_stdinc.h"
#include "SpriteSheet.h"
#include "Utils.h"
#include "tmxlite/Object.hpp"

class BoarWarrior : public Monster
{
  public:
    BoarWarrior();
    ~BoarWarrior();
    static BoarWarrior* create(const tmx::Object& data);

    // IDestroyable interface
  public:
    bool takeDamge(int damage, Direction direction) override;

    // ICollisionHandler interface
  public:
    void onPreSolve(const ContactInfo& info,
                    const b2Manifold&  oldManiflod) override;

    // GameObject interface
  public:
    void tick(float deltaTime) override;
    void paint() override;
    void cleanup() override;

  private:
    bool init(const Vec2& leftTop);
    bool init(const tmx::Object& data);
    void resetMembers();
    void idle();
    void moveForward();
    void moveBackward();
    void preAttack();
    void attack();
    void preHeavyAttack();
    void heavyAttack();
    void hurt();
    void die();
    void wait();

    void followPlayerDir();
    bool rayCast(int dis);
    enum State
    {
        STATE_IDLE,
        STATE_TRIGGER_MOVE_FORWARD,
        STATE_TRIGGER_MOVE_BACKWARD,
        STATE_PRE_ATTACK,
        STATE_ATTACK,
        STATE_PRE_HEAVY_ATTACK,
        STATE_HEAVY_ATTACK,
        STATE_WAIT,
        STATE_DIE
    };
    enum Anim
    {
        ANIM_IDLE,
        ANIM_MOVE_FORWARD,
        ANIM_MOVE_BACKWARD,
        ANIM_PRE_ATTACK,
        ANIM_ATTACK,
        ANIM_PRE_HEAVY_ATTACK,
        ANIM_HEAVY_ATTACK,
        NUM_ANIMS
    };
    SDL_Texture*      m_texture;
    Animator*         m_animator;
    SpriteSheet       m_spriteSheet;
    Direction         m_direction;
    State             m_state;
    int               m_hurtTimer;
    float             m_timer;
    bool              m_isFacingToPlayer;
    int               m_phrase;
    float             m_waitTimer;
    int               m_counter;
    Uint8             m_alpha;
    float             m_moveBackwardTime;
    static const Vec2 SIZE;
};

#endif // BOARWARRIOR_H
