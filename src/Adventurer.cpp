#include "Adventurer.h"
#include "Animation.h"
#include "Animator.h"
#include "Axe.h"
#include "BasicAttack.h"
#include "BloodStainParticle.h"
#include "CastFireball.h"
#include "Constances.h"
#include "DoubleJump.h"
#include "Fireball.h"
#include "Game.h"
#include "GameObject.h"
#include "Grenade.h"
#include "Input.h"
#include "Inventoty.h"
#include "Level.h"
#include "Math.h"
#include "ParticleSystem.h"
#include "Rect.h"
#include "SDL_assert.h"
#include "SDL_keyboard.h"
#include "SDL_scancode.h"
#include "SpriteSheet.h"
#include "Technique.h"
#include "TextureManager.h"
#include "ThrowAxe.h"
#include "ThrowGrenade.h"
#include "Utils.h"
#include "Vec.h"
#include "WorldManager.h"
#include "box2d/b2_body.h"
#include "box2d/b2_math.h"
AdventurerIdle1State      Adventurer::idle1State = AdventurerIdle1State();
AdventurerIdle2State      Adventurer::idle2State = AdventurerIdle2State();
AdventurerRunState        Adventurer::runState   = AdventurerRunState();
AdventurerJumpState       Adventurer::jumpState  = AdventurerJumpState();
AdventurerSomersaultState Adventurer::somersaultState =
    AdventurerSomersaultState();
AdventurerFallState    Adventurer::fallState    = AdventurerFallState();
AdventurerCrouchState  Adventurer::crouchState  = AdventurerCrouchState();
AdventurerHurtState    Adventurer::hurtState    = AdventurerHurtState();
AdventurerDieState     Adventurer::dieState     = AdventurerDieState();
AdventurerAirJumpState Adventurer::airJumpState = AdventurerAirJumpState();

Adventurer::Adventurer(const Vec2& center)
{
    m_body            = nullptr;
    m_animator        = nullptr;
    m_state           = nullptr;
    m_activeTechnique = nullptr;
    m_techniqueA      = nullptr;
    m_techniqueB      = nullptr;
    m_inventory       = new Inventory();
    m_identifier      = Identifier(TAG_ADVENTURER, this);

    m_abilities[ABILITY_DOUBLE_JUMP] = new DoubleJump();

    m_techniqueB = new BasicAttack();

    m_inventory->AddTechnique(m_techniqueA = new ThrowGrenade());
    m_inventory->AddTechnique(new ThrowAxe());
    m_inventory->AddTechnique(new CastFireBall());

    InitGraphicsComponent();
    InitPhysicsComponent(center);
    EnableAbilty(ABILITY_DOUBLE_JUMP);
    ResetMembers();
}

Adventurer::~Adventurer()
{
    if (m_body != nullptr)
    {
        m_body->SetUserData(nullptr);
        m_body->GetWorld()->DestroyBody(m_body);
        m_body = nullptr;
    }
    DELETE_NULL(m_animator);
    DELETE_NULL(m_inventory);
}

bool Adventurer::InitGraphicsComponent()
{
    SDL_Texture* texture = TextureManager::Get(TEX_PLAYER);
    m_spriteSheet.Init(texture, SPRITE_WIDTH, SPRITE_HEIGHT);
    Animation* anims[NUM_ANIMS];
    anims[ANIM_IDLE_1]      = new Animation(&m_spriteSheet, 0, 4, 1.f / 8.f);
    anims[ANIM_CROUCH]      = new Animation(&m_spriteSheet, 4, 4, 1.f / 8.f);
    anims[ANIM_RUN]         = new Animation(&m_spriteSheet, 8, 6, 1.f / 8.f);
    anims[ANIM_JUMP]        = new Animation(&m_spriteSheet, 14, 4, 1.f / 6.f);
    anims[ANIM_SOMERSULT]   = new Animation(&m_spriteSheet, 18, 4, 1.f / 12.f);
    anims[ANIM_FALL]        = new Animation(&m_spriteSheet, 22, 2, 1.f / 10.f);
    anims[ANIM_SLIDE]       = new Animation(&m_spriteSheet, 25, 2, 1.f / 8.f);
    anims[ANIM_STAND]       = new Animation(&m_spriteSheet, 26, 3, 1.f / 8.f);
    anims[ANIM_CORNER_GRAB] = new Animation(&m_spriteSheet, 30, 4, 1.f / 8.f);
    anims[ANIM_IDLE_2]      = new Animation(&m_spriteSheet, 38, 4, 1.f / 10.f);
    anims[ANIM_ATK_1]       = new Animation(&m_spriteSheet, 42, 5, 1.f / 12.f);
    anims[ANIM_ATK_2]       = new Animation(&m_spriteSheet, 47, 6, 1.f / 12.f);
    anims[ANIM_ATK_3]       = new Animation(&m_spriteSheet, 53, 6, 1.f / 12.f);
    anims[ANIM_HURT]        = new Animation(&m_spriteSheet, 59, 3, 1.f / 8.f);
    anims[ANIM_DIE]         = new Animation(&m_spriteSheet, 61, 7, 1.f / 8.f);
    anims[ANIM_SWORD_DRAW]  = new Animation(&m_spriteSheet, 69, 4, 1.f / 8.f);
    anims[ANIM_SWORD_SHEATHE] = new Animation(&m_spriteSheet, 73, 4, 1.f / 8.f);
    anims[ANIM_CORNER_JUMP]   = new Animation(&m_spriteSheet, 73, 2, 1.f / 8.f);
    anims[ANIM_WALL_SLIDE]    = new Animation(&m_spriteSheet, 75, 2, 1.f / 8.f);
    anims[ANIM_WALL_CLIMB]    = new Animation(&m_spriteSheet, 77, 4, 1.f / 8.f);
    anims[ANIM_CAST_SPELL] = new Animation(&m_spriteSheet, 85, 4, 1.f / 12.f);
    anims[ANIM_CAST_LOOP]  = new Animation(&m_spriteSheet, 89, 4, 1.f / 15.f);
    anims[ANIM_USE_ITEM]   = new Animation(&m_spriteSheet, 93, 3, 1.f / 8.f);
    anims[ANIM_AIR_ATK_1]  = new Animation(&m_spriteSheet, 96, 4, 1.f / 8.f);
    anims[ANIM_AIR_ATK_2]  = new Animation(&m_spriteSheet, 100, 3, 1.f / 8.f);
    anims[ANIM_AIR_ATK_3_LOOP] =
        new Animation(&m_spriteSheet, 103, 2, 1.f / 8.f);
    anims[ANIM_AIR_ATK_3_RDY] =
        new Animation(&m_spriteSheet, 106, 1, 1.f / 8.f);
    anims[ANIM_DASH] = new Animation(&m_spriteSheet, 77, 1, 1.f);

    anims[ANIM_IDLE_1]->SetPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIM_IDLE_2]->SetPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIM_RUN]->SetPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIM_FALL]->SetPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIM_SLIDE]->SetPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIM_CROUCH]->SetPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIM_CAST_LOOP]->SetPlayMode(Animation::PLAY_MODE_LOOP);

    m_animator = new Animator(anims, NUM_ANIMS);
    return true;
}

void Adventurer::InitPhysicsComponent(const Vec2& center)
{
    b2World&  world = *WorldManager::GetWorld();
    b2BodyDef bdef;
    bdef.userData      = &m_identifier;
    bdef.type          = b2_dynamicBody;
    bdef.fixedRotation = true;
    bdef.position.x    = center.x / Constances::PPM;
    bdef.position.y    = center.y / Constances::PPM;

    m_body = world.CreateBody(&bdef);

    b2PolygonShape box;
    box.SetAsBox(WIDTH / 2.f / Constances::PPM, HEIGHT_IN_METER / 2.f * 0.75f,
                 b2Vec2(0.f, -HEIGHT_IN_METER * 0.25f), 0.f);

    b2FixtureDef fdef;
    fdef.shape               = &box;
    fdef.filter.categoryBits = CATEGORY_BIT_ADVENTURER;
    fdef.filter.maskBits =
        CATEGORY_BIT_BLOCK | CATEGORY_BIT_MONSTER | CATEGORY_BIT_OTHER_OBJECT;
    fdef.friction    = 0.0f;
    fdef.restitution = 0.f;
    m_body->CreateFixture(&fdef);

    b2CircleShape circle;
    circle.m_radius = WIDTH_IN_METER / 2.f;
    circle.m_p.Set(0, HEIGHT_IN_METER / 2.f * 0.5f);

    b2FixtureDef gfdef;
    gfdef.shape               = &circle;
    gfdef.restitution         = 0.f;
    gfdef.friction            = 0.f;
    gfdef.density             = 1.f;
    gfdef.filter.categoryBits = CATEGORY_BIT_ADVENTURER;
    gfdef.filter.maskBits =
        CATEGORY_BIT_BLOCK | CATEGORY_BIT_MONSTER | CATEGORY_BIT_OTHER_OBJECT;

    m_body->CreateFixture(&gfdef);
}

void Adventurer::ResetMembers()
{
    m_direction                    = DIRECTION_RIGHT;
    m_maxHitPoints                 = 22;
    m_hitPoints                    = m_maxHitPoints;
    m_maxManaPoints                = 21;
    m_manaPoints                   = m_maxManaPoints;
    m_isGrounded                   = false;
    m_prevGroundState              = false;
    m_jumpPressedRemember          = 0.f;
    m_jumpPressedRememberTime      = 0.05f;
    m_groundedRemember             = 0.f;
    m_groundedRememberTime         = 0.5f;
    m_horizontalAcceleration       = 0.f;
    m_horizontalDampingWhenStoping = 0.9f;
    m_horizontalDampingBasic       = 0.7f;
    m_horizontalDampingWhenTurning = 0.4f;
    m_totalExtraJumps              = 1;
    m_extraJumps                   = 0;
    m_cutJumpHeight                = DEFAULT_CUT_JUMP_HEIGHT;
    m_runAcceleration              = DEFAULT_RUN_ACCELERATION;
    m_ableToUseTechnique           = true;
    m_state                        = &idle1State;
    m_vulnerable                   = true;
    EnableAbilty(ABILITY_DOUBLE_JUMP);
    m_state->Enter(*this);
    m_body->SetTransform(b2Vec2(10, 10), (float)m_rotation);
}

void Adventurer::UpdateGraphics(float deltaTime)
{
    m_animator->Tick(deltaTime);
}

void Adventurer::UpdatePhysics(float deltaTime)
{
    m_positionX       = m_body->GetPosition().x * Constances::PPM;
    m_positionY       = m_body->GetPosition().y * Constances::PPM;
    m_rotation        = m_body->GetAngle();
    m_prevGroundState = m_isGrounded;

    const int checkBoxHeight = 4;
    FloatRect box;
    box.x      = GetPositionX() - WIDTH / 2 + 2;
    box.y      = GetPositionY() + HEIGHT / 2 - checkBoxHeight / 2;
    box.width  = WIDTH - 4;
    box.height = checkBoxHeight;

    m_isGrounded = BoxCast(box, CATEGORY_BIT_BLOCK);

    b2Vec2 vel = m_body->GetLinearVelocity();
    vel.x += m_horizontalAcceleration;
    if (m_horizontalAcceleration == 0.f)
    {
        vel.x *=
            SDL_powf(1.f - m_horizontalDampingWhenStoping, deltaTime * 10.f);
    }
    else if (Math::Sign(vel.x) != Math::Sign(m_horizontalAcceleration))
    {
        vel.x *=
            SDL_powf(1.f - m_horizontalDampingWhenTurning, deltaTime * 10.f);
    }
    else
    {
        vel.x *= SDL_powf(1.f - m_horizontalDampingBasic, deltaTime * 10.f);
    }
    m_body->SetLinearVelocity(vel);
}

void Adventurer::SynchronizeBodyTransform()
{
    b2Vec2 position;
    position.x = m_positionX / Constances::PPM;
    position.y = m_positionY / Constances::PPM;
    m_body->SetTransform(position, (float)m_rotation);
}

void Adventurer::Tick(float deltaTime)
{
    UpdatePhysics(deltaTime);
    UpdateLogic(deltaTime);
    UpdateGraphics(deltaTime);
}

void Adventurer::Paint()
{
    SDL_Renderer*   renderer = GAME->GetRenderer();
    const SDL_Rect& viewport = GetScene()->GetCamera().GetViewport();
    const Sprite&   sprite   = m_animator->GetCurrentSprite();

    SDL_Rect dstrect;
    dstrect.x = m_positionX - SPRITE_WIDTH / 2 - viewport.x;
    dstrect.y = m_positionY - SPRITE_HEIGHT / 2 - viewport.y;
    dstrect.w = sprite.GetWidth();
    dstrect.h = sprite.GetHeight();

    SDL_RendererFlip flip =
        m_direction == DIRECTION_LEFT ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    sprite.Draw(renderer, &dstrect, 0.f, nullptr, flip);
}

void Adventurer::Cleanup() { delete this; }

void Adventurer::UpdateLogic(float deltaTime)
{
    if (!m_isGrounded)
    {
        m_groundedRemember -= deltaTime;
    }
    else
    {
        ResetGroundedRemember();
        ResetExtraJumps();
    }
    m_jumpPressedRemember -= deltaTime;
    if (Input::IsPressed(BUTTON_B))
    {
        ResetJumpPressedRemember();
    }

    if (m_activeTechnique != nullptr)
    {
        if (m_activeTechnique->Tick(*this, deltaTime))
        {
            m_activeTechnique->Exit(*this);
            m_activeTechnique = nullptr;
        }
        return;
    }
    if (m_ableToUseTechnique)
    {
        if (m_techniqueA != nullptr && Input::IsJustPressed(BUTTON_A) &&
            Input::IsPressed(BUTTON_PAD_UP) &&
            ConsumeMana(m_techniqueA->GetMPCost()))
        {
            m_activeTechnique = m_techniqueA;
            m_activeTechnique->Enter(*this);
            return;
        }
        if (m_techniqueB != nullptr && Input::IsJustPressed(BUTTON_A) &&
            ConsumeMana(m_techniqueB->GetMPCost()))
        {
            m_activeTechnique = m_techniqueB;
            m_activeTechnique->Enter(*this);
            return;
        }
    }
    AdventurerState* newState = m_state->Tick(*this, deltaTime);
    if (newState != nullptr)
    {
        m_state->Exit(*this);
        m_state = newState;
        m_state->Enter(*this);
    }
}

bool Adventurer::IsGrounded() const { return m_isGrounded; }

bool Adventurer::JustGrounded() const
{
    return !m_prevGroundState && m_isGrounded;
}

void Adventurer::ResetGroundedRemember()
{
    m_groundedRemember = m_jumpPressedRememberTime;
}

void Adventurer::ResetJumpPressedRemember()
{
    m_jumpPressedRemember = m_jumpPressedRememberTime;
}

void Adventurer::OnBeginContact(const ContactInfo&) {}

void Adventurer::OnEndContact(const ContactInfo&) {}

void Adventurer::OnPreSolve(const ContactInfo&, const b2Manifold&) {}

void Adventurer::OnPostSolve(const ContactInfo&, const b2ContactImpulse&) {}

bool Adventurer::TakeDamge(int damage, Direction direction)
{
    if (damage <= 0)
        return false;
    if (m_vulnerable)
    {
        Level* level = static_cast<Level*>(GetScene());
        level->GetParticleSystem()->Create<BloodStainParticle>(GetPosition());
        if (direction != DIRECTION_NONE)
        {
            m_direction = direction;
        }
        m_hitPoints -= damage;
        if (m_hitPoints <= 0)
        {
            m_hitPoints = 0;
            m_state->Exit(*this);
            m_state = &dieState;
            m_state->Enter(*this);
        }
        else
        {
            m_state->Exit(*this);
            m_state = &hurtState;
            m_state->Enter(*this);
        }
        return true;
    }
    return false;
}

int Adventurer::GetHitPoints() { return m_hitPoints; }

int Adventurer::GetMaxHitPoints() { return m_maxHitPoints; }

bool Adventurer::IsDead() { return m_hitPoints == 0; }

int  Adventurer::GetManaPoints() const { return m_manaPoints; }
int  Adventurer::GetMaxManaPoints() const { return m_maxManaPoints; }
bool Adventurer::ConsumeMana(int amount)
{
    if (m_manaPoints >= amount)
    {
        m_manaPoints -= amount;
        return true;
    }
    return false;
}

void Adventurer::OnPositionChanged() { SynchronizeBodyTransform(); }

void Adventurer::EnableAbilty(int ability)
{
    m_abilities[ability]->Enable(*this);
}

void Adventurer::DisableAbility(int ability)
{
    m_abilities[ability]->Disable(*this);
}

void Adventurer::HandleInput()
{
    if (m_activeTechnique != nullptr)
    {
        if (m_activeTechnique->HandleInput(*this))
        {
            m_activeTechnique->Exit(*this);
            m_activeTechnique = nullptr;
        }
        return;
    }
    AdventurerState* newState = m_state->HandleInput(*this);
    if (newState != nullptr)
    {
        m_state->Exit(*this);
        m_state = newState;
        m_state->Enter(*this);
    }
}

void Adventurer::ResetExtraJumps() { m_extraJumps = m_totalExtraJumps; }
