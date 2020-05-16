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
    static BoarWarrior* Create(const tmx::Object& data);

    // IDestroyable interface
  public:
    bool TakeDamge(int damage, Direction direction) override;

    // ICollisionHandler interface
  public:
    void OnPreSolve(const ContactInfo& info,
                    const b2Manifold&  oldManiflod) override;

    // GameObject interface
  public:
    void Tick(float deltaTime) override;
    void Paint() override;
    void Cleanup() override;

  private:
    bool Init(const Vec2& leftTop);
    bool Init(const tmx::Object& data);
    void ResetMembers();
    void Idle();
    void MoveForward();
    void MoveBackward();
    void PreAttack();
    void Attack();
    void PreHeavyAttack();
    void HeavyAttack();
    void Hurt();
    void Die();
    void Wait();

    void FollowAdventurerDir();
    bool RayCast(int dis);
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
        ANIm_IDLE,
        ANIm_MOVE_FORWARD,
        ANIm_MOVE_BACKWARD,
        ANIm_PRE_ATTACK,
        ANIm_ATTACK,
        ANIm_PRE_HEAVY_ATTACK,
        ANIm_HEAVY_ATTACK,
        NUM_ANIMS
    };
    SDL_Texture*      m_texture;
    Animator*         m_animator;
    SpriteSheet       m_spriteSheet;
    Direction         m_direction;
    State             m_state;
    int               m_hurtTimer;
    float             m_timer;
    bool              m_isFacingToAdventurer;
    int               m_phrase;
    float             m_waitTimer;
    int               m_counter;
    Uint8             m_alpha;
    float             m_moveBackwardTime;
    static const Vec2 SIZE;
};

#endif // BOARWARRIOR_H
