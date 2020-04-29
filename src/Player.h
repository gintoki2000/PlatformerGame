#ifndef PLAYER_H
#define PLAYER_H
#include "CollisionHandler.h"
#include "Constances.h"
#include "Destroyable.h"
#include "GameObject.h"
#include "SpriteSheet.h"
#include "Utils.h"
#include "Vec.h"
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
class Player : public GameObject, public ICollisionHandler, public IDestroyable
{

  public:
    static constexpr float WIDTH                    = 16.f;
    static constexpr float HEIGHT                   = 30.f;
    static constexpr int   SPRITE_WIDTH             = 50;
    static constexpr int   SPRITE_HEIGHT            = 37;
    static constexpr float JUMP_VEL                 = 16.f;
    static constexpr float DEFAULT_RUN_ACCELERATION = 1.f;
    static constexpr float WIDTH_IN_METER           = WIDTH / Constances::PPM;
    static constexpr float HEIGHT_IN_METER          = HEIGHT / Constances::PPM;
    static constexpr float SLIDE_SPEED              = 8.f;
    static const float     MAX_RUN_SPEED;
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
    static Player* create(const Vec2& center);
    ~Player() override;

    /// GameObject interface
  public:
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
    int          getManaPoints() const;
    int          getMaxManaPoints() const;
    bool         consumeMana(int amount);

  private:
    Player();
    bool init(const Vec2& center);
    bool initGraphicsComponent();
    void initPhysicsComponent(const Vec2& center);
    void updatePhysics(float deltaTime);
    void updateGraphics(float deltaTime);
    void updateLogic(float deltaTime);
    void synchronizeBodyTransform();

    SpriteSheet  m_spriteSheet;
    b2Body*      m_body;
    Animator*    m_animator;
    PlayerState* m_state;
    PlayerSkill* m_activeSkill;
    PlayerSkill* m_skillA;
    PlayerSkill* m_skillB;
    PlayerSkill* m_passiveSkill;
    bool         m_prevGroundState;
    bool         m_isGrounded;
    Identifier   m_identifier;

    static constexpr int NUM_SKILLS = 3;
    PlayerSkill*         m_testSkill[NUM_SKILLS];
    int                  m_currentSkill;

  public:
    Direction m_direction;
    float     m_horiziontalAcceleration;
    float     m_horizontalDampingWhenStoping;
    float     m_horizontalDampingWhenTurning;
    float     m_horizontalDampingBasic;
    int       m_hitPoints;
    int       m_manaPoints;
    int       m_maxHitPoints;
    int       m_maxManaPoints;
    float     m_jumpPressedRememberTime;
    float     m_groundedRememberTime;
    float     m_cutJumpHeight;
    float     m_jumpPressedRemember;
    float     m_groundedRemember;
    float     m_maxHoriontalSpeed;
    int       m_totalExtrasJump;
    int       m_extrasJumpCount;
    bool      m_ableToUseSkill;
    float     m_runAcceleration;
    Status    m_status;

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
    void onBeginContact(const ContactInfo& info) override;
    void onEndContact(const ContactInfo& info) override;
    void onPreSolve(const ContactInfo& info,
                    const b2Manifold&  oldManiflod) override;
    void onPostSolve(const ContactInfo&      info,
                     const b2ContactImpulse& impluse) override;

    // IDestroyable interface
  public:
    bool takeDamge(int damage, Direction direction) override;
    int  getHitPoints() override;
    int  getMaxHitPoints() override;
    bool isDead() override;

    // GameObject interface
  protected:
    void onPositionChanged() override;
};
#endif // PLAYER_H
