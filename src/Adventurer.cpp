#include "Adventurer.h"
#include "Animation.h"
#include "Animator.h"

#include "Axe.h"
#include "BasicAttack.h"
#include "BloodStainParticle.h"
#include "Constances.h"
#include "DirectionalCast.h"
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
#include "ThowAxe.h"
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
    m_identifier      = Identifier(TAG_PLAYER, this);

    m_abilities[ABILITY_DOUBLE_JUMP] = new DoubleJump();

    m_techniqueB = new BasicAttack();
	m_techniqueA = new ThrowAxe();

	m_inventory->addTechnique(new BasicAttack());
	m_inventory->addTechnique(new ThrowAxe());


    initGraphicsComponent();
    initPhysicsComponent(center);
    enableAbilty(ABILITY_DOUBLE_JUMP);
    resetMembers();
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
    DELETE_NULL(m_techniqueA);
    DELETE_NULL(m_techniqueB);
}

bool Adventurer::initGraphicsComponent()
{
    SDL_Texture* texture = TextureManager::get(TEX_PLAYER);
    m_spriteSheet.init(texture, SPRITE_WIDTH, SPRITE_HEIGHT);
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

    anims[ANIM_IDLE_1]->setPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIM_IDLE_2]->setPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIM_RUN]->setPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIM_FALL]->setPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIM_SLIDE]->setPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIM_CROUCH]->setPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIM_CAST_LOOP]->setPlayMode(Animation::PLAY_MODE_LOOP);

    m_animator = new Animator(anims, NUM_ANIMS);
    return true;
}

void Adventurer::initPhysicsComponent(const Vec2& center)
{
    b2World&  world = *WorldManager::getWorld();
    b2BodyDef bdef;
    bdef.userData      = &m_identifier;
    bdef.type          = b2_dynamicBody;
    bdef.fixedRotation = true;
    bdef.allowSleep    = false;
    bdef.position.x    = center.x / Constances::PPM;
    bdef.position.y    = center.y / Constances::PPM;

    m_body = world.CreateBody(&bdef);

    b2PolygonShape box;
    box.SetAsBox(WIDTH / 2.f / Constances::PPM, HEIGHT_IN_METER / 2.f * 0.75f,
                 b2Vec2(0.f, -HEIGHT_IN_METER * 0.25f), 0.f);

    b2FixtureDef fdef;
    fdef.shape               = &box;
    fdef.filter.categoryBits = CATEGORY_BIT_PLAYER;
    fdef.filter.maskBits     = CATEGORY_BIT_BLOCK | CATEGORY_BIT_MONSTER;
    fdef.friction            = 0.0f;
    fdef.restitution         = 0.f;
    m_body->CreateFixture(&fdef);

    b2CircleShape circle;
    circle.m_radius = WIDTH_IN_METER / 2.f;
    circle.m_p.Set(0, HEIGHT_IN_METER / 2.f * 0.5f);

    b2FixtureDef gfdef;
    gfdef.shape               = &circle;
    gfdef.restitution         = 0.f;
    gfdef.friction            = 0.f;
    gfdef.density             = 1.f;
    gfdef.filter.categoryBits = CATEGORY_BIT_PLAYER;
    gfdef.filter.maskBits     = CATEGORY_BIT_BLOCK;

    m_body->CreateFixture(&gfdef);
}

void Adventurer::resetMembers()
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
    m_totalExtrasJump              = 1;
    m_extrasJumpCount              = 0;
    m_cutJumpHeight                = DEFAULT_CUT_JUMP_HEIGHT;
    m_runAcceleration              = DEFAULT_RUN_ACCELERATION;
    m_ableToUseTechnique           = true;
    m_state                        = &idle1State;
    m_vulnerable                   = true;
    enableAbilty(ABILITY_DOUBLE_JUMP);
    m_state->enter(*this);
    m_body->SetTransform(b2Vec2(10, 10), (float)m_rotation);
}

void Adventurer::updateGraphics(float deltaTime)
{
    m_animator->tick(deltaTime);
}

void Adventurer::updatePhysics(float deltaTime)
{
    m_positionX       = m_body->GetPosition().x * Constances::PPM;
    m_positionY       = m_body->GetPosition().y * Constances::PPM;
    m_rotation        = m_body->GetAngle();
    m_prevGroundState = m_isGrounded;

    const int checkBoxHeight = 4;
    FloatRect groundCheckBox;
    groundCheckBox.x      = getPositionX() - WIDTH / 2 + 2;
    groundCheckBox.y      = getPositionY() + HEIGHT / 2 - checkBoxHeight / 2;
    groundCheckBox.width  = WIDTH - 4;
    groundCheckBox.height = checkBoxHeight;

    m_isGrounded = boxCast(groundCheckBox, CATEGORY_BIT_BLOCK);

    b2Vec2 vel = m_body->GetLinearVelocity();
    vel.x += m_horizontalAcceleration;
    if (m_horizontalAcceleration == 0.f)
    {
        vel.x *=
            SDL_powf(1.f - m_horizontalDampingWhenStoping, deltaTime * 10.f);
    }
    else if (Math::sign(vel.x) != Math::sign(m_horizontalAcceleration))
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

void Adventurer::synchronizeBodyTransform()
{
    b2Vec2 position;
    position.x = m_positionX / Constances::PPM;
    position.y = m_positionY / Constances::PPM;
    m_body->SetTransform(position, (float)m_rotation);
}

void Adventurer::tick(float deltaTime)
{
    updatePhysics(deltaTime);
    updateLogic(deltaTime);
    updateGraphics(deltaTime);
}

void Adventurer::paint()
{
    SDL_Renderer*   renderer = GAME->renderer();
    const SDL_Rect& viewport = getScene()->getCamera().getViewport();
    const Sprite&   sprite   = m_animator->getCurrentSprite();

    SDL_Rect dstrect;
    dstrect.x = m_positionX - SPRITE_WIDTH / 2 - viewport.x;
    dstrect.y = m_positionY - SPRITE_HEIGHT / 2 - viewport.y;
    dstrect.w = sprite.getWidth();
    dstrect.h = sprite.getHeight();

    SDL_RendererFlip flip =
        m_direction == DIRECTION_LEFT ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    sprite.draw(renderer, &dstrect, 0.f, nullptr, flip);
}

void Adventurer::cleanup() { delete this; }

void Adventurer::updateLogic(float deltaTime)
{
    if (!m_isGrounded)
    {
        m_groundedRemember -= deltaTime;
    }
    else
    {
        resetGroundedRemember();
        m_extrasJumpCount = m_totalExtrasJump;
    }
    m_jumpPressedRemember -= deltaTime;
    if (Input::isPressed(BUTTON_B))
    {
        resetJumpPressedRemember();
    }

    if (m_activeTechnique != nullptr)
    {
        if (m_activeTechnique->tick(*this, deltaTime))
        {
            m_activeTechnique->exit(*this);
            m_activeTechnique = nullptr;
        }
        return;
    }
    if (m_ableToUseTechnique)
    {
        if (m_techniqueA != nullptr && Input::isJustPressed(BUTTON_A) &&
            Input::isPressed(BUTTON_PAD_UP) &&
            consumeMana(m_techniqueA->getMPCost()))
        {
            m_activeTechnique = m_techniqueA;
            m_activeTechnique->enter(*this);
            return;
        }
        if (m_techniqueB != nullptr && Input::isJustPressed(BUTTON_A) &&
            consumeMana(m_techniqueB->getMPCost()))
        {
            m_activeTechnique = m_techniqueB;
            m_activeTechnique->enter(*this);
            return;
        }
    }
    AdventurerState* newState = m_state->tick(*this, deltaTime);
    if (newState != nullptr)
    {
        m_state->exit(*this);
        m_state = newState;
        m_state->enter(*this);
    }
}

bool Adventurer::isGrounded() const { return m_isGrounded; }

bool Adventurer::justGrounded() const
{
    return !m_prevGroundState && m_isGrounded;
}

void Adventurer::resetGroundedRemember()
{
    m_groundedRemember = m_jumpPressedRememberTime;
}

void Adventurer::resetJumpPressedRemember()
{
    m_jumpPressedRemember = m_jumpPressedRememberTime;
}

//**************************************************************************//
// 								States
// 																			//
//**************************************************************************//
AdventurerState::~AdventurerState() {}

void AdventurerState::enter(Adventurer&) {}

void AdventurerState::exit(Adventurer&) {}

AdventurerState* AdventurerOnGroundState::tick(Adventurer& adventurer, float)
{

    int inputDirection = Input::getHorizontalInputDirection();
    if (Input::isJustPressed(BUTTON_A))
    {
    }
    if (adventurer.m_jumpPressedRemember > 0.f)
    {
        return &Adventurer::jumpState;
    }
    if (Input::isPressed(BUTTON_PAD_DOWN))
    {
        return &Adventurer::crouchState;
    }
    else if (inputDirection != 0)
    {
        return &Adventurer::runState;
    }
    else if (!adventurer.isGrounded())
    {
        return &Adventurer::fallState;
    }
    return nullptr;
}

void AdventurerIdle1State::enter(Adventurer& adventurer)
{
    adventurer.m_horizontalAcceleration = 0.f;
    adventurer.getAnimator()->play(Adventurer::ANIM_IDLE_1, 0.f);
}

void AdventurerIdle2State::enter(Adventurer& adventurer)
{
    m_timer                             = 0.f;
    adventurer.m_horizontalAcceleration = 0;
    adventurer.getAnimator()->play(Adventurer::ANIM_IDLE_2, 0.f);
}

AdventurerState* AdventurerIdle2State::tick(Adventurer& adventurer,
                                            float       deltaTime)
{
    AdventurerState* newState =
        AdventurerOnGroundState::tick(adventurer, deltaTime);
    m_timer += deltaTime;
    if (newState != nullptr)
    {
        return newState;
    }
    else if (m_timer > 3.f)
    {
        return &Adventurer::idle1State;
    }
    return nullptr;
}

void AdventurerRunState::enter(Adventurer& adventurer)
{
    adventurer.getAnimator()->play(Adventurer::ANIM_RUN, 0.f);
}

AdventurerState* AdventurerRunState::tick(Adventurer& adventurer, float)
{
    int inputDirection = Input::getHorizontalInputDirection();
    if (inputDirection < 0)
    {
        adventurer.setDirection(DIRECTION_LEFT);
    }
    if (inputDirection > 0)
    {
        adventurer.setDirection(DIRECTION_RIGHT);
    }
    if (adventurer.m_jumpPressedRemember > 0.f)
    {
        return &Adventurer::jumpState;
    }
    else if (!adventurer.isGrounded())
    {
        return &Adventurer::fallState;
    }
    else if (inputDirection == 0)
    {
        return &Adventurer::idle1State;
    }
    else
    {
        adventurer.m_horizontalAcceleration =
            adventurer.m_runAcceleration * inputDirection;
    }
    return nullptr;
}

void AdventurerJumpState::enter(Adventurer& adventurer)
{
    b2Vec2 vel = adventurer.getBody()->GetLinearVelocity();
    vel.y      = -Adventurer::JUMP_VEL;
    adventurer.getBody()->SetLinearVelocity(vel);
    adventurer.getAnimator()->play(Adventurer::ANIM_JUMP, 0.f);
    adventurer.setUnGrounded();
    adventurer.m_jumpPressedRemember = 0.f;
    adventurer.m_groundedRemember    = 0.f;
}

AdventurerState* AdventurerJumpState::tick(Adventurer& adventurer, float)
{
    int inputDirection = Input::getHorizontalInputDirection();
    if (inputDirection < 0)
    {
        adventurer.setDirection(DIRECTION_LEFT);
    }
    if (inputDirection > 0)
    {
        adventurer.setDirection(DIRECTION_RIGHT);
    }
    if (Input::isReleased(BUTTON_B))
    {
        b2Vec2 vel = adventurer.getBody()->GetLinearVelocity();
        if (vel.y < 0.f)
        {
            vel.y *= adventurer.m_cutJumpHeight;
            adventurer.getBody()->SetLinearVelocity(vel);
        }
    }
    if (adventurer.isGrounded())
    {
        return &Adventurer::idle1State;
    }
    else if (Input::isJustPressed(BUTTON_B) && adventurer.m_extrasJumpCount > 0)
    {
        --adventurer.m_extrasJumpCount;
        return &Adventurer::airJumpState;
    }
    else if (adventurer.getBody()->GetLinearVelocity().y > 0.f)
    {
        return &Adventurer::fallState;
    }
    else if (inputDirection != 0)
    {
        adventurer.m_horizontalAcceleration =
            adventurer.m_runAcceleration * inputDirection;
    }
    else
    {
        adventurer.m_horizontalAcceleration = 0.f;
    }
    return nullptr;
}

void AdventurerSomersaultState::enter(Adventurer& adventurer)
{
    adventurer.getAnimator()->play(Adventurer::ANIM_SOMERSULT, 0.f);
    adventurer.m_ableToUseTechnique = false;
}

AdventurerState* AdventurerSomersaultState::tick(Adventurer& adventurer, float)
{

    int inputDirection = Input::getHorizontalInputDirection();
    if (inputDirection < 0)
    {
        adventurer.m_direction = DIRECTION_LEFT;
    }
    if (inputDirection > 0)
    {
        adventurer.m_direction = DIRECTION_RIGHT;
    }
    if (adventurer.isGrounded())
    {
        return &Adventurer::idle1State;
    }
    else if (adventurer.getAnimator()->isCurrentAnimationFinshed())
    {
        return &Adventurer::fallState;
    }

    if (Input::isReleased(BUTTON_B))
    {
        b2Vec2 vel = adventurer.getBody()->GetLinearVelocity();
        if (vel.y < 0.f)
        {
            vel.y *= adventurer.m_cutJumpHeight;
            adventurer.getBody()->SetLinearVelocity(vel);
        }
    }
    if (inputDirection != 0)
    {
        adventurer.m_horizontalAcceleration =
            adventurer.m_runAcceleration * inputDirection;
    }
    else
    {
        adventurer.m_horizontalAcceleration = 0.f;
    }
    return nullptr;
}

void AdventurerSomersaultState::exit(Adventurer& adventurer)
{
    adventurer.m_ableToUseTechnique = true;
}

void AdventurerFallState::enter(Adventurer& adventurer)
{
    adventurer.getAnimator()->play(Adventurer::ANIM_FALL, 0.f);
}

AdventurerState* AdventurerFallState::tick(Adventurer& adventurer, float)
{
    int inputDirection = Input::getHorizontalInputDirection();
    if (inputDirection < 0)
    {
        adventurer.setDirection(DIRECTION_LEFT);
    }
    if (inputDirection > 0)
    {
        adventurer.setDirection(DIRECTION_RIGHT);
    }
    if (adventurer.m_jumpPressedRemember > 0.f &&
        adventurer.m_groundedRemember > 0.f)
    {
        return &Adventurer::jumpState;
    }
    else if (adventurer.isGrounded())
    {
        if (inputDirection == 0)
        {
            return &Adventurer::idle1State;
        }
        else
        {
            return &Adventurer::runState;
        }
    }
    else if (Input::isJustPressed(BUTTON_B) && adventurer.m_extrasJumpCount > 0)
    {
        --adventurer.m_extrasJumpCount;
        return &Adventurer::airJumpState;
    }
    else if (inputDirection != 0)
    {
        adventurer.m_horizontalAcceleration =
            adventurer.m_runAcceleration * inputDirection;
    }
    else
    {
        adventurer.m_horizontalAcceleration = 0.f;
    }
    return nullptr;
}

void AdventurerHurtState::enter(Adventurer& adventurer)
{
    adventurer.m_horizontalAcceleration = 0.f;
    adventurer.getAnimator()->play(Adventurer::ANIM_HURT, 0.f);
    adventurer.m_ableToUseTechnique = false;
    adventurer.m_vulnerable         = false;
    adventurer.getBody()->SetLinearVelocity(b2Vec2_zero);
}

AdventurerState* AdventurerHurtState::tick(Adventurer& adventurer, float)
{
    if (adventurer.getAnimator()->isCurrentAnimationFinshed() &&
        adventurer.isGrounded())
    {
        return &Adventurer::idle1State;
    }
    return nullptr;
}

void AdventurerHurtState::exit(Adventurer& adventurer)
{
    adventurer.m_ableToUseTechnique = true;
    adventurer.m_vulnerable         = true;
}

void AdventurerDieState::enter(Adventurer& adventurer)
{
    adventurer.getAnimator()->play(Adventurer::ANIM_DIE);
    adventurer.m_ableToUseTechnique     = false;
    adventurer.m_vulnerable             = false;
    adventurer.m_horizontalAcceleration = 0.f;
    adventurer.getBody()->SetLinearVelocity(b2Vec2_zero);
}

AdventurerState* AdventurerDieState::tick(Adventurer& adventurer, float)
{
    if (adventurer.getAnimator()->isCurrentAnimationFinshed())
    {
        adventurer.resetMembers();
    }
    return nullptr;
}

void AdventurerDieState::exit(Adventurer& adventurer)
{
    adventurer.m_ableToUseTechnique = true;
    adventurer.m_vulnerable         = true;
}

void AdventurerCrouchState::enter(Adventurer& adventurer)
{
    adventurer.getAnimator()->play(Adventurer::ANIM_CROUCH);
}

AdventurerState* AdventurerCrouchState::tick(Adventurer& adventurer, float)
{
    if (Input::isReleased(BUTTON_PAD_DOWN))
    {
        return &Adventurer::idle1State;
    }
    int inputDirection = Input::getHorizontalInputDirection();
    if (inputDirection < 0)
    {
        adventurer.setDirection(DIRECTION_LEFT);
    }
    if (inputDirection > 0)
    {
        adventurer.setDirection(DIRECTION_RIGHT);
    }
    return nullptr;
}

void AdventurerAirJumpState::enter(Adventurer& adventurer)
{
    adventurer.getAnimator()->play(Adventurer::ANIM_JUMP);
    b2Vec2 vel = adventurer.getBody()->GetLinearVelocity();
    vel.y      = -Adventurer::JUMP_VEL;
    adventurer.getBody()->SetLinearVelocity(vel);
}

AdventurerState* AdventurerAirJumpState::tick(Adventurer& adventurer, float)
{
    int inputDirection = Input::getHorizontalInputDirection();
    if (inputDirection < 0)
    {
        adventurer.setDirection(DIRECTION_LEFT);
    }
    if (inputDirection > 0)
    {
        adventurer.setDirection(DIRECTION_RIGHT);
    }
    if (adventurer.getAnimator()->isCurrentAnimationFinshed())
    {
        return &Adventurer::somersaultState;
    }
    if (Input::isReleased(BUTTON_B))
    {
        b2Vec2 vel = adventurer.getBody()->GetLinearVelocity();
        if (vel.y < 0.f)
        {
            vel.y *= adventurer.m_cutJumpHeight;
            adventurer.getBody()->SetLinearVelocity(vel);
        }
    }

    if (inputDirection != 0)
    {
        adventurer.m_horizontalAcceleration =
            adventurer.m_runAcceleration * inputDirection;
    }
    else
    {
        adventurer.m_horizontalAcceleration = 0.f;
    }
    return nullptr;
}

void Adventurer::onBeginContact(const ContactInfo&) {}

void Adventurer::onEndContact(const ContactInfo&) {}

void Adventurer::onPreSolve(const ContactInfo&, const b2Manifold&) {}

void Adventurer::onPostSolve(const ContactInfo&, const b2ContactImpulse&) {}

bool Adventurer::takeDamge(int damage, Direction direction)
{
    if (damage <= 0)
        return false;
    if (m_vulnerable)
    {
        Level* level = static_cast<Level*>(getScene());
        level->getParticleSystem()->create<BloodStainParticle>(getPosition());
        if (direction != DIRECTION_NONE)
        {
            m_direction = direction;
        }
        m_hitPoints -= damage;
        if (m_hitPoints <= 0)
        {
            m_hitPoints = 0;
            m_state->exit(*this);
            m_state = &dieState;
            m_state->enter(*this);
        }
        else
        {
            m_state->exit(*this);
            m_state = &hurtState;
            m_state->enter(*this);
        }
        return true;
    }
    return false;
}

int Adventurer::getHitPoints() { return m_hitPoints; }

int Adventurer::getMaxHitPoints() { return m_maxHitPoints; }

bool Adventurer::isDead() { return m_hitPoints == 0; }

int  Adventurer::getManaPoints() const { return m_manaPoints; }
int  Adventurer::getMaxManaPoints() const { return m_maxManaPoints; }
bool Adventurer::consumeMana(int amount)
{
    if (m_manaPoints >= amount)
    {
        m_manaPoints -= amount;
        return true;
    }
    return false;
}

void Adventurer::onPositionChanged() { synchronizeBodyTransform(); }

void Adventurer::enableAbilty(int ability)
{
    m_abilities[ability]->enable(*this);
}

void Adventurer::disableAbility(int ability)
{
    m_abilities[ability]->disable(*this);
}
