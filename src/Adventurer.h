#ifndef PLAYER_H
#define PLAYER_H
#include "Ability.h"
#include "CollisionHandler.h"
#include "Constances.h"
#include "Destroyable.h"
#include "GameObject.h"
#include "Inventoty.h"
#include "SpriteSheet.h"
#include "Utils.h"
#include "Vec.h"
#include "box2d/box2d.h"
class Level;
class Animator;
class Adventurer;
class Technique;
class AdventurerState
{
  public:
    virtual ~AdventurerState();
    virtual void             enter(Adventurer& adventurer);
    virtual AdventurerState* tick(Adventurer& adventurer, float deltaTime) = 0;
    virtual void             exit(Adventurer& adventurer);
};

class AdventurerOnGroundState : public AdventurerState
{
  public:
    AdventurerState* tick(Adventurer& adventurer, float deltaTime) override;
};

class AdventurerIdle1State : public AdventurerOnGroundState
{
    void enter(Adventurer& adventurer) override;
};

class AdventurerIdle2State : public AdventurerOnGroundState
{
    void             enter(Adventurer& adventurer) override;
    AdventurerState* tick(Adventurer& adventurer, float deltaTime) override;

    float m_timer;
};

class AdventurerRunState : public AdventurerState
{
    void             enter(Adventurer& adventurer) override;
    AdventurerState* tick(Adventurer& adventurer, float deltaTime) override;
};

class AdventurerJumpState : public AdventurerState
{
    void             enter(Adventurer& adventurer) override;
    AdventurerState* tick(Adventurer& adventurer, float deltaTime) override;
};

class AdventurerSomersaultState : public AdventurerState
{
    void             enter(Adventurer& adventurer) override;
    AdventurerState* tick(Adventurer& adventurer, float deltaTime) override;
    void             exit(Adventurer& adventurer) override;
};

class AdventurerFallState : public AdventurerState
{
    void             enter(Adventurer& adventurer) override;
    AdventurerState* tick(Adventurer& adventurer, float deltaTime) override;
};

class AdventurerHurtState : public AdventurerState
{
    void             enter(Adventurer& adventurer) override;
    AdventurerState* tick(Adventurer& adventurer, float deltaTime) override;
    void             exit(Adventurer& adventurer) override;
};

class AdventurerDieState : public AdventurerState
{
    void             enter(Adventurer& adventurer) override;
    AdventurerState* tick(Adventurer& adventurer, float deltaTime) override;
    void             exit(Adventurer& adventurer) override;
};

class AdventurerCrouchState : public AdventurerState
{
    void             enter(Adventurer& adventurer) override;
    AdventurerState* tick(Adventurer& adventurer, float deltaTime) override;
};

class AdventurerAirJumpState : public AdventurerState
{
    void             enter(Adventurer& adventurer) override;
    AdventurerState* tick(Adventurer& adventurer, float deltaTime) override;
};
class Adventurer : public GameObject,
                   public ICollisionHandler,
                   public IDestroyable
{

  public:
    static constexpr float WIDTH                    = 16.f;
    static constexpr float HEIGHT                   = 30.f;
    static constexpr int   SPRITE_WIDTH             = 50;
    static constexpr int   SPRITE_HEIGHT            = 37;
    static constexpr float JUMP_VEL                 = 16.f;
    static constexpr float DEFAULT_RUN_ACCELERATION = 1.2f;
    static constexpr float WIDTH_IN_METER           = WIDTH / Constances::PPM;
    static constexpr float HEIGHT_IN_METER          = HEIGHT / Constances::PPM;
    static constexpr float SLIDE_SPEED              = 8.f;
    static constexpr float DEFAULT_CUT_JUMP_HEIGHT  = 0.5f;
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

    enum
    {
        ABILITY_DOUBLE_JUMP,
        NUM_ABILITIES
    };

  public:
    Adventurer(const Vec2& center);
    static Adventurer* create(const Vec2& center);
    ~Adventurer() override;

    /// GameObject interface
  public:
    void tick(float deltaTime) override;
    void paint() override;
    void cleanup() override;

    b2Body*    getBody() const { return m_body; }
    Animator*  getAnimator() const { return m_animator; }
    Direction  getDirection() const { return m_direction; }
    void       setDirection(Direction direction) { m_direction = direction; }
    void       resetMembers();
    Technique* getTechniqueA() const { return m_techniqueA; }
    void       setTechniqueA(Technique* technique) { m_techniqueA = technique; }
    Technique* getTechniqueB() { return m_techniqueB; }
    void       setTechniqueB(Technique* technique) { m_techniqueB = technique; }
    void       resetJumpPressedRemember();
    bool       isGrounded() const;
    bool       wasGrounded() const { return m_prevGroundState; }
    bool       justGrounded() const;
    void       resetGroundedRemember();
    void       setUnGrounded() { m_isGrounded = false; }
    int        getManaPoints() const;
    int        getMaxManaPoints() const;
    bool       consumeMana(int amount);
    void       enableAbilty(int ability);
    void       disableAbility(int ability);

  private:
    bool initGraphicsComponent();
    void initPhysicsComponent(const Vec2& center);
    void updatePhysics(float deltaTime);
    void updateGraphics(float deltaTime);
    void updateLogic(float deltaTime);
    void synchronizeBodyTransform();

    SpriteSheet      m_spriteSheet;
    b2Body*          m_body;
    Animator*        m_animator;
    AdventurerState* m_state;
    Technique*       m_activeTechnique;
    Technique*       m_techniqueA;
    Technique*       m_techniqueB;
    bool             m_prevGroundState;
    bool             m_isGrounded;
    Identifier       m_identifier;

    Ability*   m_abilities[NUM_ABILITIES];
    Inventory* m_inventory;

  public:
    Direction m_direction;
    float     m_horizontalAcceleration;
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
    bool      m_ableToUseTechnique;
    float     m_runAcceleration;
    bool      m_vulnerable;

    static AdventurerIdle1State      idle1State;
    static AdventurerIdle2State      idle2State;
    static AdventurerRunState        runState;
    static AdventurerJumpState       jumpState;
    static AdventurerSomersaultState somersaultState;
    static AdventurerFallState       fallState;
    static AdventurerCrouchState     crouchState;
    static AdventurerHurtState       hurtState;
    static AdventurerDieState        dieState;
    static AdventurerAirJumpState    airJumpState;

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
