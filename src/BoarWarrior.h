#ifndef BOARWARRIOR_H
#define BOARWARRIOR_H
#include "Animator.h"
#include "Monster.h"
#include "SpriteSheet.h"
#include "Utils.h"
#include "tmxlite/Object.hpp"

class BoarWarrior : public Monster
{
  public:
    BoarWarrior();
    ~BoarWarrior();
    static BoarWarrior* create(const tmx::Object& data);
    // static BoarWarrior* create(const Vec2& leftTop);

    // IDestroyable interface
  public:
    void takeDamge(int damage) override;

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
    void run();
    void attack();
    void heavyAttack();
    void hurt();
    void die();

    bool isFacingWithPlayer();
    enum State
    {
        STATE_IDLE,
        STATE_RUN_FORWARD,
        STATE_ATTACK,
        STATE_HEAVY_ATTACK,
        STATE_RUN_BACKWARD,
        STATE_DIE,
    };
    enum Anim
    {
        ANIM_IDLE,
        ANIM_RUN,
        ANIM_ATTACK,
        ANIM_HEAVY_ATTACK,
        NUM_ANIMS
    };
    SDL_Texture* m_texture;
    Animator*    m_animator;
    SpriteSheet  m_spriteSheet;
    Direction    m_direction;
    State        m_state;
    float        m_hurtTimer;
    float        m_timer;
    bool         m_isHitPlayer;

    static const Vec2 SIZE;
};

#endif // BOARWARRIOR_H
