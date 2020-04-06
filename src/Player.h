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
class PlayerSkill;
class PlayerState
{
  public:
    virtual ~PlayerState();

    virtual void         enter()               = 0;
    virtual PlayerState* tick(float deltaTime) = 0;

    Player* getPlayer() const { return m_player; }
    void    setPlayer(Player* player) { m_player = player; }

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

class PlayerSomersaultState : public PlayerState
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

class PlayerCrouchState : public PlayerState
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
    static constexpr int   NUM_SKILLS      = 3;
    static constexpr float CUT_JUMP_HEIGHT = 0.5f;
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
        ANIM_DASH,
        NUM_ANIMS
    };

    enum Status
    {
        STATTUS_DIE,
        STATUS_HURT,
        STATUS_NORMAL
    };

  public:
    Player(Level* level);
    ~Player() override;

    void tick(float deltaTime) override;
    void paint() override;

    bool      isTouchingWall() const;
    void      getHit(int damage);
    bool      isDead() const;
    void      setWeapon(Weapon* weapon);
    b2Body*   getBody() const { return m_body; }
    Animator* getAnimator() const { return m_animator; }
    Direction getDirection() const { return m_direction; }
    void      setDirection(Direction direction) { m_direction = direction; }
    int       getHitPoints() const { return m_hitPoints; }
    int       getManaPoints() const { return m_manaPoints; }
    int       getMaxHitPoints() const { return m_maxHitPoints; }
    void      setHorizontalSpeed(float vx);
    void      stopHorizontalMovement();
    void      stopVerticalMovement();
    void      resetMembers();
    void      move(int sign, float deltaTime);
    void      setState(PlayerState* newState);
    Status    getStatus() const { return m_status; }
    void      setStatus(Status status) { m_status = status; }
    void      setSkill(PlayerSkill* skill, int slot);
    void      getSkill(int slot);
    float     getJumpPressedRemember() const { return m_jumpPressedRemember; }
    void      resetJumpPressedRemember();
    void      setJumpPressedRemember(float time);
    void      setGroundedRemember(float time);
    bool      isGrounded() const;
    bool      wasGrounded() const { return m_prevGroundState; }
    bool      justGrounded() const;
    float     getGroundedRemember() const { return m_groundedRemember; }
    void      resetGroundedRemember();
    void      setUnGrounded() { m_isGrounded = false; }
    float     getHorizontalAcceleration() const
    {
        return m_horiziontalAcceleration;
    }
    void setHorizontalAcceleration(float horizontalAcceleration)
    {
        m_horiziontalAcceleration = horizontalAcceleration;
    }

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
    PlayerState* m_state;
    int          m_hitPoints;
    int          m_manaPoints;
    int          m_maxHitPoints;
    int          m_maxManaPoints;
    Status       m_status;
    PlayerSkill* m_skills[NUM_SKILLS];
    PlayerSkill* m_activeSkill;
    float        m_horiziontalAcceleration;
    float        m_horizontalDampingWhenStoping;
    float        m_horizontalDampingWhenTurning;
    float        m_horizontalDampingBasic;
    bool         m_prevGroundState;
    bool         m_isGrounded;
    float        m_jumpPressedRememberTime;
    float        m_groundedRememberTime;
    float        m_cutJumpHeight;
    float        m_jumpPressedRemember;
    float        m_groundedRemember;
    float        m_maxHoriontalSpeed;
};
#endif // PLAYER_H
