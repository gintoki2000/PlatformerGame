#include "Player.h"
#include "Animation.h"
#include "Animator.h"
#include "AssertManager.h"
#include "Axe.h"
#include "BloodStainParticle.h"
#include "Constances.h"
#include "DirectionalCast.h"
#include "Fireball.h"
#include "Game.h"
#include "GameObject.h"
#include "Grenade.h"
#include "Input.h"
#include "Level.h"
#include "Math.h"
#include "PlayerSkill.h"
#include "Rect.h"
#include "SDL_assert.h"
#include "SDL_keyboard.h"
#include "SDL_scancode.h"
#include "Slash.h"
#include "SpriteSheet.h"
#include "Utils.h"
#include "Vec.h"
#include "WorldManager.h"
#include "box2d/b2_body.h"
#include "ParticleSystem.h"
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
Player::Player()
{
    m_body        = nullptr;
    m_animator    = nullptr;
    m_state       = nullptr;
    m_activeSkill = nullptr;
    m_skillA      = nullptr;
    m_skillB      = nullptr;
    m_identifier  = Identifier(TAG_PLAYER, this);
}

Player::~Player()
{
    if (m_body != nullptr)
    {
        m_body->SetUserData(nullptr);
        m_body->GetWorld()->DestroyBody(m_body);
        m_body = nullptr;
    }
    for (int i = 0; i < NUM_SKILLS; ++i)
    {
        DELETE_NULL(m_testSkill[i]);
    }
    m_skillA = nullptr;
    DELETE_NULL(m_animator);
    DELETE_NULL(m_skillA);
    DELETE_NULL(m_skillB);
}

Player* Player::create(const Vec2& center)
{
    Player* ret = new Player;
    if (ret->init(center))
    {
        return ret;
    }
    DELETE_NULL(ret);
    return nullptr;
}

bool Player::init(const Vec2& center)
{
    if (!initGraphicsComponent())
    {
        return false;
    }
    initPhysicsComponent(center);
    m_testSkill[0] = DirectionalCast<Grenade>::create();
    m_testSkill[1] = DirectionalCast<Axe>::create();
    m_testSkill[2] = DirectionalCast<Fireball>::create();

    m_skillA = m_testSkill[0];
	m_skillB = Slash::create();

    resetMembers();
    return true;
}

bool Player::initGraphicsComponent()
{
    SDL_Texture* texture =
        GAME->textureMGR().getTexture(TextureManager::PLAYER);
    if (texture == nullptr)
    {
        return false;
    }
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

void Player::initPhysicsComponent(const Vec2& center)
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

void Player::resetMembers()
{
    m_direction                    = DIRECTION_RIGHT;
    m_maxHitPoints                 = 22;
    m_hitPoints                    = m_maxHitPoints;
    m_maxManaPoints                = 21;
    m_manaPoints                   = m_maxManaPoints;
    m_status                       = STATUS_NORMAL;
    m_isGrounded                   = false;
    m_prevGroundState              = false;
    m_jumpPressedRemember          = 0.f;
    m_jumpPressedRememberTime      = 0.05f;
    m_groundedRemember             = 0.f;
    m_groundedRememberTime         = 0.5f;
    m_horiziontalAcceleration      = 0.f;
    m_horizontalDampingWhenStoping = 0.9f;
    m_horizontalDampingBasic       = 0.7f;
    m_horizontalDampingWhenTurning = 0.4f;
    m_totalExtrasJump              = 0;
    m_extrasJumpCount              = 0;
    m_cutJumpHeight                = DEFAULT_CUT_JUMP_HEIGHT;
    m_runAcceleration              = DEFAULT_RUN_ACCELERATION;
    m_ableToUseSkill               = true;
    m_state                        = &idle1State;
    m_state->enter(*this);
    m_body->SetTransform(b2Vec2(10, 10), (float)m_rotation);
}

void Player::updateGraphics(float deltaTime) { m_animator->tick(deltaTime); }

void Player::updatePhysics(float deltaTime)
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

void Player::paint()
{
    SDL_Renderer*   renderer = GAME->renderer();
    const SDL_Rect& viewport = getLayerManager()->getCamera().getViewport();
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

void Player::updateLogic(float deltaTime)
{

    const Uint8* ks = SDL_GetKeyboardState(nullptr);
    if (ks[SDL_SCANCODE_I])
    {
        --m_currentSkill;
        if (m_currentSkill < 0)
        {
            m_currentSkill = NUM_SKILLS - 1;
        }
        m_skillA = m_testSkill[m_currentSkill];
    }
    else if (ks[SDL_SCANCODE_O])
    {
        ++m_currentSkill;
        if (m_currentSkill > NUM_SKILLS - 1)
        {
            m_currentSkill = 0;
        }
        m_skillA = m_testSkill[m_currentSkill];
    }

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
    if (m_ableToUseSkill && m_skillA != nullptr && m_skillA->activate(*this))
    {
        m_activeSkill = m_skillA;
        m_activeSkill->enter(*this);
    }
    else if (m_ableToUseSkill && m_skillB != nullptr &&
             m_skillB->activate(*this))
    {
        m_activeSkill = m_skillB;
        m_activeSkill->enter(*this);
    }
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
    PlayerState* newState = PlayerOnGroundState::tick(player, deltaTime);
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
    int inputDirection = Input::getHorizontalInputDirection();
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

PlayerState* PlayerSomersaultState::tick(Player& player, float)
{

    int inputDirection = Input::getHorizontalInputDirection();
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

    if (Input::isButtonBReleased())
    {
        b2Vec2 vel = player.getBody()->GetLinearVelocity();
        if (vel.y < 0.f)
        {
            vel.y *= player.m_cutJumpHeight;
            player.getBody()->SetLinearVelocity(vel);
        }
    }
    if (inputDirection != 0)
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
    player.m_status         = Player::STATUS_HURT;
    /*
b2Vec2 vel;
float  sign = player.m_direction == DIRECTION_LEFT ? -1.f : 1.f;
vel.x       = 30.f * (-sign);
vel.y       = -5.f;
player.getBody()->SetLinearVelocity(vel);
    */
}

PlayerState* PlayerHurtState::tick(Player& player, float)
{
    if (player.getAnimator()->isCurrentAnimationFinshed() &&
        player.isGrounded())
    {
        return &Player::idle1State;
    }
    return nullptr;
}

void PlayerHurtState::exit(Player& player)
{
    player.m_ableToUseSkill = true;
    player.m_status         = Player::STATUS_NORMAL;
}

void PlayerDieState::enter(Player& player)
{
    player.getAnimator()->play(Player::ANIM_DIE);
    player.m_ableToUseSkill          = false;
    player.m_status                  = Player::STATTUS_DIE;
    player.m_horiziontalAcceleration = 0.f;
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
    player.getAnimator()->play(Player::ANIM_CROUCH);
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
    player.getAnimator()->play(Player::ANIM_JUMP);
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
    if (Input::isButtonBReleased())
    {
        b2Vec2 vel = player.getBody()->GetLinearVelocity();
        if (vel.y < 0.f)
        {
            vel.y *= player.m_cutJumpHeight;
            player.getBody()->SetLinearVelocity(vel);
        }
    }

    if (inputDirection != 0)
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

void Player::onBeginContact(const ContactInfo& info)
{
    if (info.getOtherIdentifier()->tag == TAG_BLOCK)
    {
    }
}

void Player::onEndContact(const ContactInfo&) {}

void Player::onPreSolve(const ContactInfo& info, const b2Manifold&)
{
    if (info.getOtherIdentifier()->tag == TAG_BLOCK)
    {
    }
}

void Player::onPostSolve(const ContactInfo&, const b2ContactImpulse&) {}

bool Player::takeDamge(int damage, Direction direction)
{
    if (damage <= 0)
        return false;
    if (m_status == STATUS_NORMAL)
    {
        Level* level = static_cast<Level*>(getLayerManager());
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

int Player::getHitPoints() { return m_hitPoints; }

int Player::getMaxHitPoints() { return m_maxHitPoints; }

bool Player::isDead() { return m_hitPoints == 0; }

int  Player::getManaPoints() const { return m_manaPoints; }
int  Player::getMaxManaPoints() const { return m_maxManaPoints; }
bool Player::consumeMana(int amount)
{
    if (m_manaPoints >= amount)
    {
        m_manaPoints -= amount;
        return true;
    }
    return false;
}

void Player::onPositionChanged() { synchronizeBodyTransform(); }
