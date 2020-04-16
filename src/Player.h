#ifndef PLAYER_H
#define PLAYER_H
#include "CollisionCallback.h"
#include "Constances.h"
#include "Enums.h"
#include "GameObject.h"
#include "SpriteSheet.h"
#include "Utils.h"
#include "box2d/box2d.h"
class Level;
class Animator;
class Player;
class PlayerSkill;
class PlayerState
{
  public:
    virtual ~PlayerState();
    virtual void         enter(Player& player);
    virtual PlayerState* tick(Player& player, float deltaTime) = 0;
    virtual void         exit(Player& player);
};

class PlayerOnGroundState : public PlayerState
{
  public:
    PlayerState* tick(Player& player, float deltaTime) override;
};

class PlayerIdle1State : public PlayerOnGroundState
{
    void enter(Player& player) override;
};

class PlayerIdle2State : public PlayerOnGroundState
{
    void         enter(Player& player) override;
    PlayerState* tick(Player& player, float deltaTime) override;

    float m_timer;
};

class PlayerRunState : public PlayerState
{
    void         enter(Player& player) override;
    PlayerState* tick(Player& player, float deltaTime) override;
};

class PlayerJumpState : public PlayerState
{
    void         enter(Player& player) override;
    PlayerState* tick(Player& player, float deltaTime) override;
};

class PlayerSomersaultState : public PlayerState
{
    void         enter(Player& player) override;
    PlayerState* tick(Player& player, float deltaTime) override;
    void         exit(Player& player) override;
};

class PlayerFallState : public PlayerState
{
    void         enter(Player& player) override;
    PlayerState* tick(Player& player, float deltaTime) override;
};

class PlayerHurtState : public PlayerState
{
    void         enter(Player& player) override;
    PlayerState* tick(Player& player, float deltaTime) override;
    void         exit(Player& player) override;
};

class PlayerDieState : public PlayerState
{
    void         enter(Player& player) override;
    PlayerState* tick(Player& player, float deltaTime) override;
    void         exit(Player& player) override;
};

class PlayerCrouchState : public PlayerState
{
    void         enter(Player& player) override;
    PlayerState* tick(Player& player, float deltaTime) override;
};

class PlayerAirJumpState : public PlayerState
{
    void         enter(Player& player) override;
    PlayerState* tick(Player& player, float deltaTime) override;
};
class Player : public GameObject, public ICollisionCallback
{

  public:
    static constexpr float WIDTH                    = 16.f;
    static constexpr float HEIGHT                   = 30.f;
    static constexpr int   SPRITE_WIDTH             = 50;
    static constexpr int   SPRITE_HEIGHT            = 37;
    static constexpr float JUMP_VEL                 = 16.f;
    static constexpr float DEFAULT_RUN_ACCELERATION = 2.f;
    static constexpr float WIDTH_IN_METER           = WIDTH / Constances::PPM;
    static constexpr float HEIGHT_IN_METER          = HEIGHT / Constances::PPM;
    static constexpr float SLIDE_SPEED              = 8.f;
    static const float     MAX_RUN_SPEED;
    static constexpr int   NUM_SKILLS              = 3;
    static constexpr float DEFAULT_CUT_JUMP_HEIGHT = 0.5f;
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
    Player();
    ~Player() override;

    void tick(float deltaTime) override;
    void paint() override;

    b2Body*      getBody() const { return m_body; }
    Animator*    getAnimator() const { return m_animator; }
    Direction    getDirection() const { return m_direction; }
    void         setDirection(Direction direction) { m_direction = direction; }
    void         resetMembers();
    PlayerSkill* getSkillA() const { return m_skillA; }
    void         setSkillA(PlayerSkill* skill) { m_skillA = skill; }
    PlayerSkill* getSkillB() { return m_skillB; }
    void         setSkillB(PlayerSkill* skill) { m_skillB = skill; }
    void         resetJumpPressedRemember();
    bool         isGrounded() const;
    bool         wasGrounded() const { return m_prevGroundState; }
    bool         justGrounded() const;
    void         resetGroundedRemember();
    void         setUnGrounded() { m_isGrounded = false; }

  private:
    bool initGraphicsComponent();
    void initPhysicsComponent(b2World& world);
    void updatePhysics(float deltaTime);
    void updateGraphics(float deltaTime);
    void updateLogic(float deltaTime);
    void synchronizeBodyTransform();

    /// asserts
    SpriteSheet* m_spriteSheet;

    b2Body*      m_body;
    Animator*    m_animator;
    PlayerState* m_state;
    int          m_hitPoints;
    int          m_manaPoints;
    int          m_maxHitPoints;
    int          m_maxManaPoints;
    Status       m_status;
    PlayerSkill* m_activeSkill;
    PlayerSkill* m_skillA;
    PlayerSkill* m_skillB;
    bool         m_prevGroundState;
    bool         m_isGrounded;
    Identifier   m_identifier;

  public:
    Direction m_direction;
    float     m_horiziontalAcceleration;
    float     m_horizontalDampingWhenStoping;
    float     m_horizontalDampingWhenTurning;
    float     m_horizontalDampingBasic;

    float m_jumpPressedRememberTime;
    float m_groundedRememberTime;
    float m_cutJumpHeight;
    float m_jumpPressedRemember;
    float m_groundedRemember;
    float m_maxHoriontalSpeed;
    int   m_totalExtrasJump;
    int   m_extrasJumpCount;
    bool  m_ableToUseSkill;
    float m_runAcceleration;

    static PlayerIdle1State      idle1State;
    static PlayerIdle2State      idle2State;
    static PlayerRunState        runState;
    static PlayerJumpState       jumpState;
    static PlayerSomersaultState somersaultState;
    static PlayerFallState       fallState;
    static PlayerCrouchState     crouchState;
    static PlayerHurtState       hurtState;
    static PlayerDieState        dieState;
    static PlayerAirJumpState    airJumpState;

    // ICollisionCallback interface
  public:
    virtual void onBeginContact(const ContactInfo& info) override;
    virtual void onEndContact(const ContactInfo& info) override;
    virtual void onPreSolve(const ContactInfo& info,
                            const b2Manifold&  oldManiflod) override;
    virtual void onPostSolve(const ContactInfo&      info,
                             const b2ContactImpulse& impluse) override;
};
#endif // PLAYER_H
