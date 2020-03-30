#ifndef PLAYER_H
#define PLAYER_H
#include "Constances.h"
#include "Enums.h"
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

    Player* getPlayer() const { return m_player; }
    void    setPlayer(Player* player) { m_player = player; }
    void    changePlayerDirection(int inputDirection);

  protected:
    Player* m_player;
};

class PlayerOnGroundState : public PlayerState
{
  public:
    PlayerState* tick(float deltaTime) override;
};

class PlayerIdle1State : public PlayerOnGroundState
{
    void enter() override;
};

class PlayerIdle2State : public PlayerOnGroundState
{
    void         enter() override;
    PlayerState* tick(float deltaTime) override;

    float m_timer;
};

class PlayerRunState : public PlayerState
{
    void         enter() override;
    PlayerState* tick(float deltaTime) override;
};

class PlayerJumpState : public PlayerState
{
    void         enter() override;
    PlayerState* tick(float deltaTime) override;
};

class PlayerSomersultState : public PlayerState
{
    void         enter() override;
    PlayerState* tick(float deltaTime) override;
};

class PlayerFallState : public PlayerState
{
    void         enter() override;
    PlayerState* tick(float deltaTime) override;
};

class PlayerHurtState : public PlayerState
{
    void         enter() override;
    PlayerState* tick(float deltaTime) override;
};

class PlayerDieState : public PlayerState
{
    void         enter() override;
    PlayerState* tick(float deltaTime) override;
};

class Player : public GameObject
{

  public:
    static constexpr float WIDTH           = 16.f;
    static constexpr float HEIGHT          = 30.f;
    static constexpr int   SPRITE_WIDTH    = 50;
    static constexpr int   SPRITE_HEIGHT   = 37;
    static constexpr float JUMP_VEL        = 13.f;
    static constexpr float RUN_ACC         = 0.8f * 60.f;
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
	Player(Level* level);
    ~Player();

    void      tick(float deltaTime) override;
    void      paint() override;
    void      touchGround();
    void      untouchGround();
    void      touchWall();
    void      untouchWall();
    bool      isOnGround() const;
    bool      isTouchingWall() const;
    void      getHit(int damage);
    bool      isDead() const;
    Weapon*   getWeapon() const { return m_weapon; }
    void      setWeapon(Weapon* weapon);
    b2Body*   getBody() const { return m_body; }
    Animator* getAnimator() const { return m_animator; }
    Direction getDirection() const { return m_direction; }
    void      setDirection(Direction direction) { m_direction = direction; }
    int       getHitPoints() const { return m_hitPoints; }
    int       getManaPoints() const { return m_manaPoints; }
    void      setHorizontalSpeed(float vx);
    void      stopHorizontalMovement();
    void      stopVerticalMovement();
    void      resetMembers();
    void      move(int sign, float deltaTime);
    void      setUnGround() { m_touchingGroundCount = 0; }
    void      setState(PlayerState* newState);
    void      setProtected(bool v) { m_isProtected = v; }
    bool      isProtected() const { return m_isProtected; }

  private:
    bool initGraphicsComponent();
    void initPhysicsComponent();
    void updatePhysics();
    void updateGraphics(float deltaTime);
    void updateLogic(float deltaTime);
    void onPositionChanged() override;
    void synchronizeBodyTransform();
    void synchronizeAnimatorTransform();

    /// asserts
    SpriteSheet* m_spriteSheet;

    Direction    m_direction;
    bool         m_isWallSliding;
    int          m_touchingGroundCount;
    int          m_touchingWallCount;
    b2Body*      m_body;
    Animator*    m_animator;
    Weapon*      m_weapon;
    Spell*       m_spell;
    PlayerState* m_state;
    int          m_hitPoints;
    int          m_manaPoints;
    int          m_maxHitPoints;
    int          m_maxManaPoints;
    bool         m_isProtected;
};
#endif // PLAYER_H
