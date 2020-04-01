#include "Player.h"
#include "Animation.h"
#include "Animator.h"
#include "Constances.h"
#include "Enums.h"
#include "GameObject.h"
#include "Input.h"
#include "Level.h"
#include "Locator.h"
#include "SDL_image.h"
#include "SDL_render.h"
#include "Sword.h"
#include "Weapon.h"
const float Player::MAX_RUN_SPEED = 8.f;
Player::Player(Level* level): GameObject(GAME_OBJECT_TYPE_PLAYER, level)
{
    m_body        = nullptr;
    m_spriteSheet = nullptr;
    m_animator    = nullptr;
    m_weapon      = nullptr;
    m_spell       = nullptr;
    m_state       = nullptr;

    initGraphicsComponent();
    initPhysicsComponent();
    setWeapon(new Sword());
    resetMembers();
}

Player::~Player()
{
    m_body->GetWorld()->DestroyBody(m_body);
    delete m_animator;
    delete m_spriteSheet;
    delete m_weapon;
    delete m_state;

    m_body        = nullptr;
    m_animator    = nullptr;
    m_spriteSheet = nullptr;
}


bool Player::initGraphicsComponent()
{
    auto texture = m_level->getTextureManager()->get("asserts/spritesheets/player.png");
    m_spriteSheet = new SpriteSheet(texture, SPRITE_WIDTH, SPRITE_HEIGHT);
    Animation* anims[NUM_OF_ANIMS];
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

    anims[ANIM_IDLE_1]->setPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIM_IDLE_2]->setPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIM_RUN]->setPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIM_FALL]->setPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIM_SLIDE]->setPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIM_CROUCH]->setPlayMode(Animation::PLAY_MODE_LOOP);

    m_animator = new Animator(anims, NUM_OF_ANIMS);
    m_animator->setOriginX(SPRITE_WIDTH / 2);
    m_animator->setOriginY(SPRITE_HEIGHT / 2);
    return true;
}

void Player::setWeapon(Weapon* weapon)
{
    if (m_weapon != nullptr)
    {
        delete weapon;
    }
    m_weapon           = weapon;
    m_weapon->m_player = this;
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

    m_body->CreateFixture(&fdef);

    box.SetAsBox(WIDTH / 2.f / Constances::PPM, 2.f / Constances::PPM,
                 b2Vec2(0.f, HEIGHT / 2.f / Constances::PPM), 0.f);

    b2FixtureDef footSensorfdef;
    footSensorfdef.isSensor            = true;
    footSensorfdef.shape               = &box;
    footSensorfdef.userData            = (void*)FIXTURE_TYPE_FOOT_SENSOR;
    footSensorfdef.filter.categoryBits = CATEGORY_BIT_PLAYER;
    footSensorfdef.filter.maskBits     = CATEGORY_BIT_BLOCK;

    m_body->CreateFixture(&footSensorfdef);
}

void Player::resetMembers()
{
    m_direction           = DIRECTION_RIGHT;
    m_touchingGroundCount = 0;
    m_touchingWallCount   = 0;
    m_maxHitPoints        = 5;
    m_hitPoints           = m_maxHitPoints;
    m_maxManaPoints       = 50;
    m_manaPoints          = m_maxManaPoints;
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

void Player::updatePhysics()
{
    m_positionX = m_body->GetPosition().x * Constances::PPM;
    m_positionY = m_body->GetPosition().y * Constances::PPM;
    m_rotation  = m_body->GetAngle();
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
    if (!isDead() && m_weapon->tick(deltaTime))
    {
        return;
    }

    setState(m_state->tick(deltaTime));
}

bool Player::isDead() const { return m_hitPoints == 0; }

bool Player::isOnGround() const { return m_touchingGroundCount > 0; }

void Player::touchGround() { ++m_touchingGroundCount; }

void Player::untouchGround()
{
    --m_touchingGroundCount;
    if (m_touchingGroundCount < 0)
    {
        m_touchingGroundCount = 0;
    }
}

void Player::getHit(int damage) {
	SDL_Log("player get hit!");
	if (!isDead() && !isProtected())
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
void Player::setState(PlayerState *newState)
{
	if (newState != nullptr)
	{
		delete m_state;
		m_state = newState;
		m_state->setPlayer(this);
		m_state->enter();
	}
}
//**************************************************************************//
// 								States
// 																			//
//**************************************************************************//
PlayerState::~PlayerState() {}

void PlayerState::changePlayerDirection(int inputDirection)
{
	if (inputDirection < 0)
	{
		m_player->setDirection(DIRECTION_LEFT);
	}
	if (inputDirection > 0)
	{
		m_player->setDirection(DIRECTION_RIGHT);
	}
}

PlayerState* PlayerOnGroundState::tick(float)
{

    auto inputDirection = Input::getHorizontalInputDirection();
	if (Input::isButtonAJustPressed())
	{
		m_player->getWeapon()->start();
	}
    if (Input::isButtonBPressed() && m_player->isOnGround())
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
	else if (!m_player->isOnGround())
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
	changePlayerDirection(inputDirection);
    if (Input::isButtonBJustPressed() && m_player->isOnGround())
    {
        return new PlayerJumpState;
    }
    if (inputDirection == 0)
    {
        return new PlayerIdle1State;
    }
    else
    {
        m_player->move(inputDirection, deltaTime);
        return nullptr;
    }
}

void PlayerJumpState::enter()
{
    b2Vec2 vel = m_player->getBody()->GetLinearVelocity();
    vel.y      = -Player::JUMP_VEL;
    m_player->getBody()->SetLinearVelocity(vel);
    m_player->getAnimator()->play(Player::ANIM_JUMP, 0.f);
	m_player->setUnGround();
}

PlayerState* PlayerJumpState::tick(float deltaTime)
{
    auto inputDirection = Input::getHorizontalInputDirection();
	changePlayerDirection(inputDirection);
    if (m_player->getAnimator()->isCurrentAnimationFinshed())
    {
        return new PlayerSomersaultState;
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
	changePlayerDirection(inputDirection);
    if (m_player->getAnimator()->isCurrentAnimationFinshed())
    {
        return new PlayerFallState;
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
	
    auto inputDirection = Input::getHorizontalInputDirection();
	changePlayerDirection(inputDirection);
    if (m_player->isOnGround())
    {
        if (inputDirection == 0)
		{
			return new PlayerIdle1State;
		}
		else 
		{
			return new PlayerRunState;
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
	m_player->setProtected(true);
	m_player->stopHorizontalMovement();
	m_player->getAnimator()->play(Player::ANIM_HURT, 0.f);
}

PlayerState* PlayerHurtState::tick(float)
{
	if (m_player->getAnimator()->isCurrentAnimationFinshed())
	{
		m_player->setProtected(false);
		if (m_player->isOnGround())
		{
			return new PlayerIdle1State;
		}
		else 
		{
			return new PlayerFallState;
		}
	}
	return nullptr;
}

void PlayerDieState::enter()
{
	m_player->getAnimator()->play(Player::ANIM_DIE, 0.f);
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
    changePlayerDirection(Input::getHorizontalInputDirection());
    if (Input::isButtonUpPressed())
    {
        return new PlayerIdle1State();
    }
    return nullptr;
}
