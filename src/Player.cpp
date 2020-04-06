#include "Player.h"
#include "Animation.h"
#include "Animator.h"
#include "Constances.h"
#include "Enums.h"
#include "GameObject.h"
#include "Input.h"
#include "Level.h"
#include "Locator.h"
#include "Math.h"
#include "PlayerCutSkill.h"
#include "PlayerDashSkill.h"
#include "PlayerFireBallSkill.h"
#include "PlayerSkill.h"
#include "SDL_image.h"
#include "SDL_render.h"
const float Player::MAX_RUN_SPEED = 8.f;
Player::Player(Level* level) : GameObject(GAME_OBJECT_TYPE_PLAYER, level)
{
    m_body        = nullptr;
    m_spriteSheet = nullptr;
    m_animator    = nullptr;
    m_state       = nullptr;

    for (int i = 0; i < NUM_SKILLS; ++i)
    {
        m_skills[i] = nullptr;
    }
    m_activeSkill = nullptr;

    initGraphicsComponent();
    initPhysicsComponent();
    resetMembers();

    setSkill(new PlayerFireballSkill(), 0);
    setSkill(new PlayerCutSkill(), 1);
}

Player::~Player()
{
    m_body->GetWorld()->DestroyBody(m_body);
    delete m_animator;
    delete m_spriteSheet;
    delete m_state;
    for (int i = 0; i < NUM_SKILLS; ++i)
    {
        delete m_skills[i];
    }

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
    anims[ANIM_JUMP]          = new Animation(m_spriteSheet, 14, 4, 1.f / 8.f);
    anims[ANIM_SOMERSULT]     = new Animation(m_spriteSheet, 18, 4, 1.f / 10.f);
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
    anims[ANIM_CAST_SPELL]    = new Animation(m_spriteSheet, 85, 4, 1.f / 8.f);
    anims[ANIM_CAST_LOOP]     = new Animation(m_spriteSheet, 89, 4, 1.f / 8.f);
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
    box.SetAsBox(WIDTH / 2.f / Constances::PPM, HEIGHT / 2.f / Constances::PPM);

    b2FixtureDef fdef;
    fdef.shape               = &box;
    fdef.filter.categoryBits = CATEGORY_BIT_PLAYER;
    fdef.filter.maskBits     = CATEGORY_BIT_BLOCK;
    fdef.friction            = 0.1f;
}

void Player::resetMembers()
{
    m_direction               = DIRECTION_RIGHT;
    m_touchingGroundCount     = 0;
    m_touchingWallCount       = 0;
    m_maxHitPoints            = 5;
    m_hitPoints               = m_maxHitPoints;
    m_maxManaPoints           = 50;
    m_manaPoints              = m_maxManaPoints;
    m_status                  = STATUS_NORMAL;
    m_isGrounded              = false;
    m_prevGroundState         = false;
    m_jumpPressedRemember     = 0.f;
    m_jumpPressedRememberTime = 0.3f;
    m_groundedRemember        = 0.f;
    m_groundedRememberTime    = 1.5f;
    m_horiziontalAcceleration = 0.f;
    setState(new PlayerIdle1State());
    setPosition(100.f, 0.f);
    m_body->SetAwake(true);
}

void Player::updateGraphics(float deltaTime)
{
    m_animator->tick(deltaTime);
    m_animator->setFlip(m_direction == DIRECTION_LEFT ? SDL_FLIP_HORIZONTAL
                                                      : SDL_FLIP_NONE);
    synchronizeAnimatorTransform();
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

void Player::updatePhysics()
{
    m_positionX       = m_body->GetPosition().x * Constances::PPM;
    m_positionY       = m_body->GetPosition().y * Constances::PPM;
    m_rotation        = m_body->GetAngle();
    m_prevGroundState = m_isGrounded;
    // m_isOnGround      = m_touchingGroundCount > 0;

    b2AABB groundBox;
    groundBox.lowerBound.x = m_body->GetPosition().x - WIDTH_IN_METER / 2.f;
    groundBox.lowerBound.y =
        m_body->GetPosition().y + HEIGHT_IN_METER / 2.f - 1.f / Constances::PPM;
    groundBox.upperBound.x = m_body->GetPosition().x + WIDTH_IN_METER / 2.f;
    groundBox.upperBound.y =
        m_body->GetPosition().y + HEIGHT_IN_METER / 2.f + 1.f / Constances::PPM;

    m_isGrounded =
        testOverlap(m_level->getWorld(), groundBox, CATEGORY_BIT_BLOCK);

    b2Vec2 vel = m_body->GetLinearVelocity();
    vel.x += m_horiziontalAcceleration;
    vel.x = Math::clamp(-MAX_RUN_SPEED, MAX_RUN_SPEED, vel.x);
}

void Player::synchronizeBodyTransform()
{
    b2Vec2 position;
    position.x = m_positionX / Constances::PPM;
    position.y = m_positionY / Constances::PPM;
    m_body->SetTransform(position, (float)m_rotation);
}

void Player::synchronizeAnimatorTransform()
{
    const auto& viewport = m_level->getViewport();
    m_animator->setPositionX(m_positionX - viewport.x);
    m_animator->setPositionY(m_positionY - viewport.y);
    m_animator->setRotation(m_rotation);
}

void Player::onPositionChanged()
{
    synchronizeBodyTransform();
    synchronizeAnimatorTransform();
}

void Player::tick(float deltaTime)
{
    updatePhysics();
    updateLogic(deltaTime);
    updateGraphics(deltaTime);
}

void Player::paint() { m_animator->paint(Locator::getRenderer()); }

void Player::setHorizontalSpeed(float vx)
{
    b2Vec2 vel = m_body->GetLinearVelocity();
    vel.x      = vx;
    m_body->SetLinearVelocity(vel);
}

void Player::stopHorizontalMovement() { setHorizontalSpeed(0.f); }

void Player::stopVerticalMovement()
{

    b2Vec2 vel = m_body->GetLinearVelocity();
    vel.y      = 0.f;
    m_body->SetLinearVelocity(vel);
}

void Player::updateLogic(float deltaTime)
{
    if (!m_isGrounded)
    {
        m_groundedRemember -= deltaTime;
    }
    if (m_isGrounded)
    {
        resetGroundedRemember();
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
            m_activeSkill = nullptr;
        }
        return;
    }
    for (int i = 0; i < NUM_SKILLS; ++i)
    {
        if (m_skills[i] != nullptr && m_skills[i]->activate(*this))
        {
            m_activeSkill = m_skills[i];
            return;
        }
    }
    setState(m_state->tick(deltaTime));
}

bool Player::isDead() const { return m_hitPoints == 0; }

bool Player::isGrounded() const { return m_isGrounded; }

void Player::getHit(int damage)
{
    if (m_status == STATUS_NORMAL)
    {
        m_hitPoints -= damage;
        if (m_hitPoints <= 0)
        {
            m_hitPoints = 0;
            setState(new PlayerDieState());
        }
        else
        {
            setState(new PlayerHurtState());
        }
    }
}

void Player::move(int sign, float deltaTime)
{

    b2Vec2 vel = m_body->GetLinearVelocity();
    vel.x += sign * Player::RUN_ACC * deltaTime;
    vel.x = std::max(-Player::MAX_RUN_SPEED,
                     std::min(Player::MAX_RUN_SPEED, vel.x));
    m_body->SetLinearVelocity(vel);
    m_body->SetAwake(true);
}
void Player::setState(PlayerState* newState)
{
    if (newState != nullptr)
    {
        delete m_state;
        m_state = newState;
        m_state->setPlayer(this);
        m_state->enter();
    }
}

void Player::setSkill(PlayerSkill* skill, int slot)
{
    SDL_assert(slot >= 0 && slot < NUM_SKILLS);
    if (m_skills[slot] != nullptr)
    {
        delete m_skills[slot];
    }
    m_skills[slot] = skill;
}

bool Player::justGrounded() const { return !m_prevGroundState && m_isGrounded; }

void Player::resetGroundedRemember()
{
    m_groundedRemember = m_jumpPressedRememberTime;
}

void Player::resetJumpPressedRemember()
{
    m_jumpPressedRemember = m_jumpPressedRememberTime;
}

void Player::setGroundedRemember(float time) { m_groundedRemember = time; }

void Player::setJumpPressedRemember(float time)
{
    m_jumpPressedRemember = time;
}
//**************************************************************************//
// 								States
// 																			//
//**************************************************************************//
PlayerState::~PlayerState() {}

PlayerState* PlayerOnGroundState::tick(float)
{

    auto inputDirection = Input::getHorizontalInputDirection();
    if (Input::isButtonAJustPressed())
    {
    }
    if (m_player->getJumpPressedRemember() > 0.f &&
        m_player->getGroundedRemember())
    {
        return new PlayerJumpState();
    }
    if (Input::isButtonDownPressed())
    {
        return new PlayerCrouchState();
    }
    else if (inputDirection != 0)
    {
        return new PlayerRunState();
    }
    else if (!m_player->isGrounded())
    {
        return new PlayerFallState();
    }
    return nullptr;
}

void PlayerIdle1State::enter()
{
    m_player->stopHorizontalMovement();
    m_player->getAnimator()->play(Player::ANIM_IDLE_1, 0.f);
}

void PlayerIdle2State::enter()
{
    m_timer = 0.f;
    m_player->stopHorizontalMovement();
    m_player->getAnimator()->play(Player::ANIM_IDLE_2, 0.f);
}

PlayerState* PlayerIdle2State::tick(float deltaTime)
{
    auto newState = PlayerOnGroundState::tick(deltaTime);
    m_timer += deltaTime;
    if (newState != nullptr)
    {
        return newState;
    }
    else if (m_timer > 3.f)
    {
        return new PlayerIdle1State;
    }
    return nullptr;
}

void PlayerRunState::enter()
{
    m_player->getAnimator()->play(Player::ANIM_RUN, 0.f);
}

PlayerState* PlayerRunState::tick(float deltaTime)
{
    auto inputDirection = Input::getHorizontalInputDirection();
    if (inputDirection < 0)
    {
        m_player->setDirection(DIRECTION_LEFT);
    }
    if (inputDirection > 0)
    {
        m_player->setDirection(DIRECTION_RIGHT);
    }
    if (m_player->getJumpPressedRemember() > 0.f &&
        m_player->getGroundedRemember())
    {
        return new PlayerJumpState();
    }
    else if (!m_player->isGrounded())
    {
        return new PlayerFallState();
    }
    else if (inputDirection == 0)
    {
        return new PlayerIdle1State();
    }
    else
    {
        m_player->move(inputDirection, deltaTime);
    }
    return nullptr;
}

void PlayerJumpState::enter()
{
    b2Vec2 vel = m_player->getBody()->GetLinearVelocity();
    vel.y      = -Player::JUMP_VEL;
    m_player->getBody()->SetLinearVelocity(vel);
    m_player->getAnimator()->play(Player::ANIM_JUMP, 0.f);
    m_player->setUnGrounded();
    m_player->setJumpPressedRemember(0.f);
    m_player->setGroundedRemember(0.f);
}

PlayerState* PlayerJumpState::tick(float deltaTime)
{
    auto inputDirection = Input::getHorizontalInputDirection();
    if (inputDirection < 0)
    {
        m_player->setDirection(DIRECTION_LEFT);
    }
    if (inputDirection > 0)
    {
        m_player->setDirection(DIRECTION_RIGHT);
    }
    if (Input::isButtonBReleased())
    {
        b2Vec2 vel = m_player->getBody()->GetLinearVelocity();
        if (vel.y < 0.f)
        {
            vel.y *= Player::CUT_JUMP_HEIGHT;
            m_player->getBody()->SetLinearVelocity(vel);
        }
    }
    if (m_player->isGrounded())
    {
        return new PlayerIdle1State();
    }
    else if (m_player->getAnimator()->isCurrentAnimationFinshed())
    {
        return new PlayerSomersaultState();
    }
    else if (inputDirection != 0)
    {
        m_player->move(inputDirection, deltaTime);
    }
    else
    {
        m_player->stopHorizontalMovement();
    }
    return nullptr;
}

void PlayerSomersaultState::enter()
{
    m_player->getAnimator()->play(Player::ANIM_SOMERSULT, 0.f);
}

PlayerState* PlayerSomersaultState::tick(float deltaTime)
{

    auto inputDirection = Input::getHorizontalInputDirection();
    if (inputDirection < 0)
    {
        m_player->setDirection(DIRECTION_LEFT);
    }
    if (inputDirection > 0)
    {
        m_player->setDirection(DIRECTION_RIGHT);
    }
    if (m_player->isGrounded())
    {
        return new PlayerIdle1State();
    }
    else if (m_player->getAnimator()->isCurrentAnimationFinshed())
    {
        return new PlayerFallState();
    }
    else if (inputDirection != 0)
    {
        m_player->move(inputDirection, deltaTime);
    }
    else
    {
        m_player->stopHorizontalMovement();
    }
    return nullptr;
}

void PlayerFallState::enter()
{
    m_player->getAnimator()->play(Player::ANIM_FALL, 0.f);
}

PlayerState* PlayerFallState::tick(float deltaTime)
{
    int inputDirection = Input::getHorizontalInputDirection();
    if (inputDirection < 0)
    {
        m_player->setDirection(DIRECTION_LEFT);
    }
    if (inputDirection > 0)
    {
        m_player->setDirection(DIRECTION_RIGHT);
    }
    if (m_player->getJumpPressedRemember() > 0.f &&
        m_player->getGroundedRemember() > 0.f)
    {
        return new PlayerJumpState();
    }
    else if (m_player->isGrounded())
    {
        if (inputDirection == 0)
        {
            return new PlayerIdle1State();
        }
        else
        {
            return new PlayerRunState();
        }
    }
    else if (inputDirection != 0)
    {
        m_player->move(inputDirection, deltaTime);
    }
    else
    {
        m_player->stopHorizontalMovement();
    }
    return nullptr;
}

void PlayerHurtState::enter()
{
    m_player->setStatus(Player::STATUS_HURT);
    m_player->stopHorizontalMovement();
    m_player->getAnimator()->play(Player::ANIM_HURT, 0.f);
}

PlayerState* PlayerHurtState::tick(float)
{
    if (m_player->getAnimator()->isCurrentAnimationFinshed())
    {
        m_player->setStatus(Player::STATUS_NORMAL);
        if (m_player->isGrounded())
        {
            return new PlayerIdle1State();
        }
        else
        {
            return new PlayerFallState();
        }
    }
    return nullptr;
}

void PlayerDieState::enter()
{
    m_player->getAnimator()->play(Player::ANIM_DIE, 0.f);
    m_player->setStatus(Player::STATTUS_DIE);
}

PlayerState* PlayerDieState::tick(float)
{
    if (m_player->getAnimator()->isCurrentAnimationFinshed())
    {
        m_player->resetMembers();
    }
    return nullptr;
}

void PlayerCrouchState::enter()
{
    m_player->getAnimator()->play(Player::ANIM_CROUCH, 0.f);
}

PlayerState* PlayerCrouchState::tick(float)
{
    auto inputDirection = Input::getHorizontalInputDirection();
    if (inputDirection < 0)
    {
        m_player->setDirection(DIRECTION_LEFT);
    }
    if (inputDirection > 0)
    {
        m_player->setDirection(DIRECTION_RIGHT);
    }
    if (Input::isButtonUpPressed())
    {
        return new PlayerIdle1State();
    }
    return nullptr;
}
