#include "Player.h"
#include "Animation.h"
#include "Animator.h"
#include "Audio.h"
#include "Constances.h"
#include "Enums.h"
#include "GameObject.h"
#include "Input.h"
#include "Level.h"
#include "Locator.h"
#include "Math.h"
#include "PlayerCutSkill.h"
#include "PlayerFireBallSkill.h"
#include "PlayerSkill.h"
#include "SDL_image.h"
#include "SDL_render.h"
#include "box2d/b2_body.h"
const float           Player::MAX_RUN_SPEED   = 8.f;
PlayerIdle1State      Player::idle1State      = PlayerIdle1State();
PlayerIdle2State      Player::idle2State      = PlayerIdle2State();
PlayerRunState        Player::runState        = PlayerRunState();
PlayerJumpState       Player::jumpState       = PlayerJumpState();
PlayerSomersaultState Player::somersaultState = PlayerSomersaultState();
PlayerFallState       Player::fallState       = PlayerFallState();
PlayerCrouchState     Player::crouchState     = PlayerCrouchState();
PlayerHurtState       Player::hurtState       = PlayerHurtState();
PlayerDieState        Player::dieState        = PlayerDieState();
PlayerAirJumpState    Player::airJumpState    = PlayerAirJumpState();
Player::Player(Level* level) : GameObject(level)
{
    m_body        = nullptr;
    m_spriteSheet = nullptr;
    m_animator    = nullptr;
    m_state       = nullptr;
    m_activeSkill = nullptr;

    initGraphicsComponent();
    initPhysicsComponent();
    resetMembers();

    setSkillA(new PlayerFireballSkill());
    setSkillB(new PlayerCutSkill());
}

Player::~Player()
{
    m_body->GetWorld()->DestroyBody(m_body);
    delete m_animator;
    delete m_spriteSheet;
    delete m_skillA;
    delete m_skillB;

    m_body        = nullptr;
    m_animator    = nullptr;
    m_spriteSheet = nullptr;
    m_activeSkill = nullptr;
}

bool Player::initGraphicsComponent()
{
    auto texture =
        m_level->getTextureManager()->get("asserts/spritesheets/player.png");
    SDL_assert(texture != nullptr);
    m_spriteSheet = new SpriteSheet(texture, SPRITE_WIDTH, SPRITE_HEIGHT);
    Animation* anims[NUM_ANIMS];
    anims[ANIM_IDLE_1]        = new Animation(m_spriteSheet, 0, 4, 1.f / 8.f);
    anims[ANIM_CROUCH]        = new Animation(m_spriteSheet, 4, 4, 1.f / 8.f);
    anims[ANIM_RUN]           = new Animation(m_spriteSheet, 8, 6, 1.f / 8.f);
    anims[ANIM_JUMP]          = new Animation(m_spriteSheet, 14, 4, 1.f / 6.f);
    anims[ANIM_SOMERSULT]     = new Animation(m_spriteSheet, 18, 4, 1.f / 12.f);
    anims[ANIM_FALL]          = new Animation(m_spriteSheet, 22, 2, 1.f / 10.f);
    anims[ANIM_SLIDE]         = new Animation(m_spriteSheet, 25, 2, 1.f / 8.f);
    anims[ANIM_STAND]         = new Animation(m_spriteSheet, 26, 3, 1.f / 8.f);
    anims[ANIM_CORNER_GRAB]   = new Animation(m_spriteSheet, 30, 4, 1.f / 8.f);
    anims[ANIM_IDLE_2]        = new Animation(m_spriteSheet, 38, 4, 1.f / 10.f);
    anims[ANIM_ATK_1]         = new Animation(m_spriteSheet, 42, 5, 1.f / 12.f);
    anims[ANIM_ATK_2]         = new Animation(m_spriteSheet, 47, 6, 1.f / 12.f);
    anims[ANIM_ATK_3]         = new Animation(m_spriteSheet, 53, 6, 1.f / 12.f);
    anims[ANIM_HURT]          = new Animation(m_spriteSheet, 59, 3, 1.f / 8.f);
    anims[ANIM_DIE]           = new Animation(m_spriteSheet, 61, 7, 1.f / 8.f);
    anims[ANIM_SWORD_DRAW]    = new Animation(m_spriteSheet, 69, 4, 1.f / 8.f);
    anims[ANIM_SWORD_SHEATHE] = new Animation(m_spriteSheet, 73, 4, 1.f / 8.f);
    anims[ANIM_CORNER_JUMP]   = new Animation(m_spriteSheet, 73, 2, 1.f / 8.f);
    anims[ANIM_WALL_SLIDE]    = new Animation(m_spriteSheet, 75, 2, 1.f / 8.f);
    anims[ANIM_WALL_CLIMB]    = new Animation(m_spriteSheet, 77, 4, 1.f / 8.f);
    anims[ANIM_CAST_SPELL]    = new Animation(m_spriteSheet, 85, 4, 1.f / 12.f);
    anims[ANIM_CAST_LOOP]     = new Animation(m_spriteSheet, 89, 4, 1.f / 15.f);
    anims[ANIM_USE_ITEM]      = new Animation(m_spriteSheet, 93, 3, 1.f / 8.f);
    anims[ANIM_AIR_ATK_1]     = new Animation(m_spriteSheet, 96, 4, 1.f / 8.f);
    anims[ANIM_AIR_ATK_2]     = new Animation(m_spriteSheet, 100, 3, 1.f / 8.f);
    anims[ANIM_AIR_ATK_3_LOOP] =
        new Animation(m_spriteSheet, 103, 2, 1.f / 8.f);
    anims[ANIM_AIR_ATK_3_RDY] = new Animation(m_spriteSheet, 106, 1, 1.f / 8.f);
    anims[ANIM_DASH]          = new Animation(m_spriteSheet, 77, 1, 1.f);

    anims[ANIM_IDLE_1]->setPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIM_IDLE_2]->setPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIM_RUN]->setPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIM_FALL]->setPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIM_SLIDE]->setPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIM_CROUCH]->setPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIM_CAST_LOOP]->setPlayMode(Animation::PLAY_MODE_LOOP);

    m_animator = new Animator(anims, NUM_ANIMS);
    m_animator->setOwner(this);
    m_animator->setOriginX(SPRITE_WIDTH / 2);
    m_animator->setOriginY(SPRITE_HEIGHT / 2);
    return true;
}

void Player::initPhysicsComponent()
{
    b2BodyDef bdef;
    bdef.userData      = this;
    bdef.type          = b2_dynamicBody;
    bdef.fixedRotation = true;

    m_body = m_level->getWorld()->CreateBody(&bdef);

    b2PolygonShape box;
    box.SetAsBox(WIDTH / 2.f / Constances::PPM, HEIGHT_IN_METER / 2.f * 0.75f,
                 b2Vec2(0.f, -HEIGHT_IN_METER * 0.25f), 0.f);

    b2FixtureDef fdef;
    fdef.shape               = &box;
    fdef.filter.categoryBits = CATEGORY_BIT_PLAYER;
    fdef.filter.maskBits     = CATEGORY_BIT_BLOCK;
    fdef.friction            = 0.0f;
    fdef.restitution         = 0.f;
    fdef.isSensor            = true;
    m_body->CreateFixture(&fdef);

    b2CircleShape circle;
    circle.m_radius = WIDTH_IN_METER / 2.f;
    circle.m_p.Set(0, HEIGHT_IN_METER / 2.f * 0.5f);

    b2FixtureDef gfdef;
    gfdef.shape               = &circle;
    gfdef.restitution         = 0.f;
    gfdef.friction            = 0.f;
    gfdef.filter.categoryBits = CATEGORY_BIT_PLAYER;
    gfdef.filter.maskBits     = CATEGORY_BIT_BLOCK;

    m_body->CreateFixture(&gfdef);
}

void Player::resetMembers()
{
    m_direction                    = DIRECTION_RIGHT;
    m_maxHitPoints                 = 5;
    m_hitPoints                    = m_maxHitPoints;
    m_maxManaPoints                = 50;
    m_manaPoints                   = m_maxManaPoints;
    m_status                       = STATUS_NORMAL;
    m_isGrounded                   = false;
    m_prevGroundState              = false;
    m_jumpPressedRemember          = 0.f;
    m_jumpPressedRememberTime      = 0.3f;
    m_groundedRemember             = 0.f;
    m_groundedRememberTime         = 1.5f;
    m_horiziontalAcceleration      = 0.f;
    m_horizontalDampingWhenStoping = 1.f;
    m_horizontalDampingBasic       = 0.4f;
    m_horizontalDampingWhenTurning = 0.4f;
    m_totalExtrasJump              = 1;
    m_extrasJumpCount              = 0;
    m_cutJumpHeight                = DEFAULT_CUT_JUMP_HEIGHT;
    m_runAcceleration              = DEFAULT_RUN_ACCELERATION;
    m_ableToUseSkill               = true;
    m_state                        = &idle1State;
    m_state->enter(*this);
    // setPosition(100.f, 0.f);
    m_body->SetAwake(true);
    m_body->SetTransform(b2Vec2(10, 0), (float)m_rotation);
}

void Player::updateGraphics(float deltaTime)
{
    m_animator->tick(deltaTime);
    m_animator->setFlip(m_direction == DIRECTION_LEFT ? SDL_FLIP_HORIZONTAL
                                                      : SDL_FLIP_NONE);
}

struct OverlapTestCallback : public b2QueryCallback
{
    uint16     bitMasks;
    b2Fixture* fixture;
    OverlapTestCallback()
    {
        bitMasks = 0;
        fixture  = nullptr;
    }

    bool ReportFixture(b2Fixture* f) override
    {
        if (f->GetFilterData().categoryBits & bitMasks)
        {
            this->fixture = f;
            return false;
        }
        return true;
    }
};

static bool testOverlap(b2World* world, const b2AABB& box, uint16 bitMasks)
{
    OverlapTestCallback callback;
    callback.bitMasks = bitMasks;
    world->QueryAABB(&callback, box);
    return callback.fixture != nullptr;
}

void Player::updatePhysics(float deltaTime)
{
    m_positionX       = m_body->GetPosition().x * Constances::PPM;
    m_positionY       = m_body->GetPosition().y * Constances::PPM;
    m_rotation        = m_body->GetAngle();
    m_prevGroundState = m_isGrounded;

    b2AABB groundBox;
    groundBox.lowerBound.x =
        m_body->GetPosition().x - WIDTH_IN_METER / 2.f + 5.f / Constances::PPM;
    groundBox.lowerBound.y =
        m_body->GetPosition().y + HEIGHT_IN_METER / 2.f - 1.f / Constances::PPM;
    groundBox.upperBound.x =
        m_body->GetPosition().x + WIDTH_IN_METER / 2.f - 5.f / Constances::PPM;
    groundBox.upperBound.y =
        m_body->GetPosition().y + HEIGHT_IN_METER / 2.f + 1.f / Constances::PPM;

    m_isGrounded =
        testOverlap(m_level->getWorld(), groundBox, CATEGORY_BIT_BLOCK);

    b2Vec2 vel = m_body->GetLinearVelocity();
    vel.x += m_horiziontalAcceleration;
    if (m_horiziontalAcceleration == 0.f)
    {
        vel.x *=
            SDL_powf(1.f - m_horizontalDampingWhenStoping, deltaTime * 10.f);
    }
    else if (Math::sign(vel.x) != Math::sign(m_horiziontalAcceleration))
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

void Player::synchronizeBodyTransform()
{
    b2Vec2 position;
    position.x = m_positionX / Constances::PPM;
    position.y = m_positionY / Constances::PPM;
    m_body->SetTransform(position, (float)m_rotation);
}

void Player::tick(float deltaTime)
{
    updatePhysics(deltaTime);
    updateLogic(deltaTime);
    updateGraphics(deltaTime);
}

void Player::paint() { m_animator->paint(); }

void Player::updateLogic(float deltaTime)
{
    if (!m_isGrounded)
    {
        m_groundedRemember -= deltaTime;
    }
    if (m_isGrounded)
    {
        resetGroundedRemember();
        m_extrasJumpCount = m_totalExtrasJump;
    }
    m_jumpPressedRemember -= deltaTime;
    if (Input::isButtonBPressed())
    {
        resetJumpPressedRemember();
    }
    if (m_activeSkill != nullptr)
    {
        if (m_activeSkill->tick(*this, deltaTime))
        {
            m_activeSkill->exit(*this);
            m_activeSkill = nullptr;
        }
        return;
    }
    if (m_skillA->activate(*this))
    {
        m_activeSkill = m_skillA;
        m_activeSkill->enter(*this);
    }
    else if (m_skillB->activate(*this))
    {
        m_activeSkill = m_skillB;
        m_activeSkill->enter(*this);
    }
    /*
    if (m_skillA->activate(*this))
    {
        m_activeSkill = m_skillA;
        m_activeSkill->enter(*this);
    }*/
    PlayerState* newState = m_state->tick(*this, deltaTime);
    if (newState != nullptr)
    {
        m_state->exit(*this);
        m_state = newState;
        m_state->enter(*this);
    }
}

bool Player::isGrounded() const { return m_isGrounded; }

bool Player::justGrounded() const { return !m_prevGroundState && m_isGrounded; }

void Player::resetGroundedRemember()
{
    m_groundedRemember = m_jumpPressedRememberTime;
}

void Player::resetJumpPressedRemember()
{
    m_jumpPressedRemember = m_jumpPressedRememberTime;
}

void Player::onBeginContact(b2Contact* contact, b2Fixture* other)
{
}

void Player::onEndContact(b2Contact* contact, b2Fixture* other)
{
}

//**************************************************************************//
// 								States
// 																			//
//**************************************************************************//
PlayerState::~PlayerState() {}

void PlayerState::enter(Player&) {}

void PlayerState::exit(Player&) {}

PlayerState* PlayerOnGroundState::tick(Player& player, float)
{

    int inputDirection = Input::getHorizontalInputDirection();
    if (Input::isButtonAJustPressed())
    {
    }
    if (player.m_jumpPressedRemember > 0.f)
    {
        return &Player::jumpState;
    }
    if (Input::isButtonDownPressed())
    {
        return &Player::crouchState;
    }
    else if (inputDirection != 0)
    {
        return &Player::runState;
    }
    else if (!player.isGrounded())
    {
        return &Player::fallState;
    }
    return nullptr;
}

void PlayerIdle1State::enter(Player& player)
{
    player.m_horiziontalAcceleration = 0.f;
    player.getAnimator()->play(Player::ANIM_IDLE_1, 0.f);
}

void PlayerIdle2State::enter(Player& player)
{
    m_timer                          = 0.f;
    player.m_horiziontalAcceleration = 0;
    player.getAnimator()->play(Player::ANIM_IDLE_2, 0.f);
}

PlayerState* PlayerIdle2State::tick(Player& player, float deltaTime)
{
    auto newState = PlayerOnGroundState::tick(player, deltaTime);
    m_timer += deltaTime;
    if (newState != nullptr)
    {
        return newState;
    }
    else if (m_timer > 3.f)
    {
        return &Player::idle1State;
    }
    return nullptr;
}

void PlayerRunState::enter(Player& player)
{
    player.getAnimator()->play(Player::ANIM_RUN, 0.f);
}

PlayerState* PlayerRunState::tick(Player& player, float)
{
    auto inputDirection = Input::getHorizontalInputDirection();
    if (inputDirection < 0)
    {
        player.setDirection(DIRECTION_LEFT);
    }
    if (inputDirection > 0)
    {
        player.setDirection(DIRECTION_RIGHT);
    }
    if (player.m_jumpPressedRemember > 0.f)
    {
        return &Player::jumpState;
    }
    else if (!player.isGrounded())
    {
        return &Player::fallState;
    }
    else if (inputDirection == 0)
    {
        return &Player::idle1State;
    }
    else
    {
        player.m_horiziontalAcceleration =
            player.m_runAcceleration * inputDirection;
    }
    return nullptr;
}

void PlayerJumpState::enter(Player& player)
{
    b2Vec2 vel = player.getBody()->GetLinearVelocity();
    vel.y      = -Player::JUMP_VEL;
    player.getBody()->SetLinearVelocity(vel);
    player.getAnimator()->play(Player::ANIM_JUMP, 0.f);
    player.setUnGrounded();
    player.m_jumpPressedRemember = 0.f;
    player.m_groundedRemember    = 0.f;
}

PlayerState* PlayerJumpState::tick(Player& player, float)
{
    auto inputDirection = Input::getHorizontalInputDirection();
    if (inputDirection < 0)
    {
        player.setDirection(DIRECTION_LEFT);
    }
    if (inputDirection > 0)
    {
        player.setDirection(DIRECTION_RIGHT);
    }
    if (Input::isButtonBReleased())
    {
        b2Vec2 vel = player.getBody()->GetLinearVelocity();
        if (vel.y < 0.f)
        {
            vel.y *= player.m_cutJumpHeight;
            player.getBody()->SetLinearVelocity(vel);
        }
    }
    if (player.isGrounded())
    {
        return &Player::idle1State;
    }
    else if (Input::isButtonBJustPressed() && player.m_extrasJumpCount > 0)
    {
        --player.m_extrasJumpCount;
        return &Player::airJumpState;
    }
    else if (player.getBody()->GetLinearVelocity().y > 0.f)
    {
        return &Player::fallState;
    }
    else if (inputDirection != 0)
    {
        player.m_horiziontalAcceleration =
            player.m_runAcceleration * inputDirection;
    }
    else
    {
        player.m_horiziontalAcceleration = 0.f;
    }
    return nullptr;
}

void PlayerSomersaultState::enter(Player& player)
{
    player.getAnimator()->play(Player::ANIM_SOMERSULT, 0.f);
    player.m_ableToUseSkill = false;
}

PlayerState* PlayerSomersaultState::tick(Player& player, float deltaTime)
{

    auto inputDirection = Input::getHorizontalInputDirection();
    if (inputDirection < 0)
    {
        player.m_direction = DIRECTION_LEFT;
    }
    if (inputDirection > 0)
    {
        player.m_direction = DIRECTION_RIGHT;
    }
    if (player.isGrounded())
    {
        return &Player::idle1State;
    }
    else if (player.getAnimator()->isCurrentAnimationFinshed())
    {
        return &Player::fallState;
    }
    else if (inputDirection != 0)
    {
        player.m_horiziontalAcceleration =
            player.m_runAcceleration * inputDirection;
    }
    else
    {
        player.m_horiziontalAcceleration = 0.f;
    }
    return nullptr;
}

void PlayerSomersaultState::exit(Player& player)
{
    player.m_ableToUseSkill = true;
}

void PlayerFallState::enter(Player& player)
{
    player.getAnimator()->play(Player::ANIM_FALL, 0.f);
}

PlayerState* PlayerFallState::tick(Player& player, float)
{
    int inputDirection = Input::getHorizontalInputDirection();
    if (inputDirection < 0)
    {
        player.setDirection(DIRECTION_LEFT);
    }
    if (inputDirection > 0)
    {
        player.setDirection(DIRECTION_RIGHT);
    }
    if (player.m_jumpPressedRemember > 0.f && player.m_groundedRemember > 0.f)
    {
        return &Player::jumpState;
    }
    else if (player.isGrounded())
    {
        Locator::getAudio().play(Audio::LANDING);
        if (inputDirection == 0)
        {
            return &Player::idle1State;
        }
        else
        {
            return &Player::runState;
        }
    }
    else if (Input::isButtonBJustPressed() && player.m_extrasJumpCount > 0)
    {
        --player.m_extrasJumpCount;
        return &Player::airJumpState;
    }
    else if (inputDirection != 0)
    {
        player.m_horiziontalAcceleration =
            player.m_runAcceleration * inputDirection;
    }
    else
    {
        player.m_horiziontalAcceleration = 0.f;
    }
    return nullptr;
}

void PlayerHurtState::enter(Player& player)
{
    player.m_horiziontalAcceleration = 0.f;
    player.getAnimator()->play(Player::ANIM_HURT, 0.f);
    player.m_ableToUseSkill = false;
}

PlayerState* PlayerHurtState::tick(Player& player, float)
{
    if (player.getAnimator()->isCurrentAnimationFinshed())
    {
        if (player.isGrounded())
        {
            return &Player::idle1State;
        }
        else
        {
            return &Player::fallState;
        }
    }
    return nullptr;
}

void PlayerHurtState::exit(Player& player) { player.m_ableToUseSkill = true; }

void PlayerDieState::enter(Player& player)
{
    player.getAnimator()->play(Player::ANIM_DIE, 0.f);
    player.m_ableToUseSkill = false;
}

PlayerState* PlayerDieState::tick(Player& player, float)
{
    if (player.getAnimator()->isCurrentAnimationFinshed())
    {
        player.resetMembers();
    }
    return nullptr;
}

void PlayerDieState::exit(Player& player) { player.m_ableToUseSkill = true; }

void PlayerCrouchState::enter(Player& player)
{
    player.getAnimator()->play(Player::ANIM_CROUCH, 0.f);
}

PlayerState* PlayerCrouchState::tick(Player& player, float)
{
    auto inputDirection = Input::getHorizontalInputDirection();
    if (inputDirection < 0)
    {
        player.setDirection(DIRECTION_LEFT);
    }
    if (inputDirection > 0)
    {
        player.setDirection(DIRECTION_RIGHT);
    }
    if (Input::isButtonUpPressed())
    {
        return &Player::idle1State;
    }
    return nullptr;
}

void PlayerAirJumpState::enter(Player& player)
{
    player.getAnimator()->play(Player::ANIM_JUMP, 0.f);
    b2Vec2 vel = player.getBody()->GetLinearVelocity();
    vel.y      = -Player::JUMP_VEL;
    player.getBody()->SetLinearVelocity(vel);
}

PlayerState* PlayerAirJumpState::tick(Player& player, float)
{
    int inputDirection = Input::getHorizontalInputDirection();
    if (inputDirection < 0)
    {
        player.setDirection(DIRECTION_LEFT);
    }
    if (inputDirection > 0)
    {
        player.setDirection(DIRECTION_RIGHT);
    }
    if (player.getAnimator()->isCurrentAnimationFinshed())
    {
        return &Player::somersaultState;
    }
    return nullptr;
}
