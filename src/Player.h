#ifndef PLAYER_H
#define PLAYER_H
#include "Constances.h"
#include "GameObject.h"
#include "NTLayer.h"
#include "SpriteSheet.h"
#include "box2d/box2d.h"
class Level;
class Animator;
class Weapon;
class Spell;
class Player;
class PlayerState
{
  public:
    virtual ~PlayerState();
    virtual void         enter()               = 0;
    virtual PlayerState* tick(float deltaTime) = 0;

  protected:
    friend class Player;
    Player* m_player;
};

class PlayerOnGroundState : public PlayerState
{
  public:
    PlayerState* tick(float deltaTime) override;
};

class PlayerIdle1 : public PlayerOnGroundState
{
  public:
    void enter() override;
};

class PlayerRun : public PlayerOnGroundState
{
  public:
    void         enter() override;
    PlayerState* tick(float deltaTime) override;
};

class PlayerJump : public PlayerState
{
  public:
    void         enter() override;
    PlayerState* tick(float deltaTime) override;
};

class PlayerSomersult : public PlayerState
{
  public:
    void         enter() override;
    PlayerState* tick(float deltaTime) override;
};

class PlayerFall : public PlayerState
{
    void         enter() override;
    PlayerState* tick(float deltaTime) override;
};
class Player : public GameObject
{
  private:
    enum State
    {
        STATE_IDLE_1,
        STATE_IDLE_2,
        STATE_RUN,
        STATE_JUMP,
        STATE_SOMERSULT,
        STATE_SLIDE,
        STATE_STAND,
        STATE_CORNER_GRAB,
        STATE_CORNER_JUMP,
        STATE_HURT,
        STATE_DIE,
        STATE_SWORD_DRAW,
        STATE_SWORD_SHEATHE,
        STATE_WALL_SLIDE,
        STATE_WALL_CLIMB,
        STATE_FALL,
        STATE_USE_ITEM
    };
    enum
    {
        ANIM_IDLE_1,
        ANIM_IDLE_2,
        ANIM_CROUCH,
        ANIM_RUN,
        ANIM_JUMP,
        ANIM_SOMERSULT,
        ANIM_FALL,
        ANIM_SLIDE,
        ANIM_STAND,
        ANIM_CORNER_GRAB,
        ANIM_CORNER_JUMP,
        ANIM_HURT,
        ANIM_DIE,
        ANIM_SWORD_DRAW,
        ANIM_SWORD_SHEATHE,
        ANIM_WALL_SLIDE,
        ANIM_WALL_CLIMB,
        ANIM_CAST_SPELL,
        ANIM_CAST_LOOP,
        ANIM_ATK_1,
        ANIM_ATK_2,
        ANIM_ATK_3,
        ANIM_USE_ITEM,
        ANIM_AIR_ATK_1,
        ANIM_AIR_ATK_2,
        ANIM_AIR_ATK_3_LOOP,
        ANIM_AIR_ATK_3_RDY,
        NUM_OF_ANIMS
    };

  public:
    static constexpr float WIDTH           = 16.f;
    static constexpr float HEIGHT          = 30.f;
    static constexpr int   SPRITE_WIDTH    = 50;
    static constexpr int   SPRITE_HEIGHT   = 37;
    static constexpr float JUMP_VEL        = 13.f;
    static constexpr float RUN_ACC         = 0.8f;
    static constexpr float WIDTH_IN_METER  = WIDTH / Constances::PPM;
    static constexpr float HEIGHT_IN_METER = HEIGHT / Constances::PPM;
    static constexpr float SLIDE_SPEED     = 8.f;
    static const float     MAX_RUN_SPEED;
    enum
    {
        FIXTURE_TYPE_MAIN_BODY,
        FIXTURE_TYPE_FOOT_SENSOR,
        FIXTURE_TYPE_WALL_SENSOR
    };

  public:
    static Player* create(Level* level);

    ~Player();

    void tick(float deltaTime) override;
    void paint() override;
    void touchGround();
    void untouchGround();
    void touchWall();
    void untouchWall();
    bool isOnGround() const;
    bool isTouchingWall() const;
    void getHit(int damage);
    bool isDead() const;
    void setWeapon(Weapon* weapon);

  private:
    Player();
    bool init(Level* level);

    bool initGraphicsComponent();
    void initPhysicsComponent();
    void resetMembers();
    void updatePhysics();
    void updateGraphics(float deltaTime);
    void updateLogic(float deltaTime);
    void setHorizontalSpeed(float vx);
    void stopHorizontalMovement();
    void stopVerticalMovement();
    void onPositionChanged() override;
    void synchronizeBodyTransform();
    void synchronizeAnimatorTransform();

    float        m_timer;
    int          m_direction;
    State        m_prevState;
    bool         m_continueAttack;
    bool         m_isWallSliding;
    int          m_touchingGroundCount;
    int          m_touchingWallCount;
    b2Body*      m_body;
    SDL_Texture* m_texture;
    SpriteSheet* m_spriteSheet;
    Animator*    m_animator;
    Weapon*      m_weapon;
    Spell*       m_spell;
    PlayerState* m_state;
    int          m_hitPoints;
    int          m_manaPoints;
    int          m_maxHitPoints;
    int          m_maxManaPoints;

    friend class Weapon;
    friend class Sword;
    friend class PlayerState;
    friend class PlayerRun;
    friend class PlayerOnGroundState;
    friend class PlayerIdle1;
    friend class PlayerJump;
	friend class PlayerSomersult;
	friend class PlayerFall;
};
#endif // PLAYER_H
