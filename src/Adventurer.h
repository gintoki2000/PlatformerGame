#ifndef PLAYER_H
#define PLAYER_H
#include "Ability.h"
#include "AdventureState.h"
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
    static Adventurer* Create(const Vec2& center);
    ~Adventurer() override;

    /// GameObject interface
  public:
    void Tick(float deltaTime) override;
    void Paint() override;
    void Cleanup() override;

    b2Body*    GetBody() const { return m_body; }
    Animator*  GetAnimator() const { return m_animator; }
    Direction  GetDirection() const { return m_direction; }
    void       SetDirection(Direction direction) { m_direction = direction; }
    void       ResetMembers();
    Technique* GetTechniqueA() const { return m_techniqueA; }
    void       SetTechniqueA(Technique* technique) { m_techniqueA = technique; }
    Technique* GetTechniqueB() { return m_techniqueB; }
    void       SetTechniqueB(Technique* technique) { m_techniqueB = technique; }
    void       ResetJumpPressedRemember();
    bool       IsGrounded() const;
    bool       WasGrounded() const { return m_prevGroundState; }
    bool       JustGrounded() const;
    void       ResetGroundedRemember();
    void       SetUnGrounded() { m_isGrounded = false; }
    int        GetManaPoints() const;
    int        GetMaxManaPoints() const;
    bool       ConsumeMana(int amount);
    void       EnableAbilty(int ability);
    void       DisableAbility(int ability);
    void       HandleInput();
    Inventory* GetInventory() const { return m_inventory; }
    void       ResetExtraJumps();

  private:
    bool InitGraphicsComponent();
    void InitPhysicsComponent(const Vec2& center);
    void UpdatePhysics(float deltaTime);
    void UpdateGraphics(float deltaTime);
    void UpdateLogic(float deltaTime);
    void SynchronizeBodyTransform();

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
    int       m_totalExtraJumps;
    int       m_extraJumps;
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
    void OnBeginContact(const ContactInfo& info) override;
    void OnEndContact(const ContactInfo& info) override;
    void OnPreSolve(const ContactInfo& info,
                    const b2Manifold&  oldManiflod) override;
    void OnPostSolve(const ContactInfo&      info,
                     const b2ContactImpulse& impluse) override;

    // IDestroyable interface
  public:
    bool TakeDamge(int damage, Direction direction) override;
    int  GetHitPoints() override;
    int  GetMaxHitPoints() override;
    bool IsDead() override;

    // GameObject interface
  protected:
    void OnPositionChanged() override;
};
#endif // PLAYER_H
