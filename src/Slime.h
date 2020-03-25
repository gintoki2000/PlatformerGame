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
    static Slime* create(Level* level);
    static Slime* create(Level* level, float cx, float cy);
    ~Slime();

    void getHit(int damage) override;

    void tick(float deltaTime) override;
    void paint() override;

  private:
    bool init(Level*);
    void resetMembers();
    void updateGraphics(float deltaTime);
    void updatePhysics();
    void updateLogic(float deltaTime);
    void onPositionChanged() override;
    void setHorizontalSpeed(float speed);
    void stopHorizontalMovement();
    void attackPlayer();
    void synchronizeBodyTransform();
	void checkDirection();

    void idle();
    void wait();
    void move();
    void attack();
    void hurt();
    void die();
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

    void      setState(State newState, float initialTime);
    State     m_state;
    float     m_timer;
    float     m_changingDirTimer;
    int       m_direction;
    b2Body*   m_body;
    Animator* m_animator;

    static constexpr float WIDTH             = 32.f;
    static constexpr float HEIGHT            = 25.f;
    static constexpr int   SPRITE_WIDTH      = 32;
    static constexpr int   SPRITE_HEIGHT     = 25;
    static constexpr float ACTIVATE_DISTANCE = Constances::TILE_SIZE * 5;
    static constexpr float ATTACK_DISTANCE   = Constances::TILE_SIZE * 1;
    static constexpr float MOVE_SPEED        = 1.f;
    static constexpr float MOVE_SPPED_2      = 15.f;

    struct Asserts
    {
        Asserts();
        ~Asserts();

        bool load();
        void incCount();
        void decCount();

        SDL_Texture* texture;
        SpriteSheet* spriteSheet;
        int          count;
    };
    static Asserts sharedAsserts;
};
#endif // SLIME_H
