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
const float Player::MAX_RUN_SPEED = 7.f;
Player::Player()
{
    m_body        = nullptr;
    m_texture     = nullptr;
    m_spriteSheet = nullptr;
    m_animator    = nullptr;
    m_weapon      = nullptr;
    m_spell       = nullptr;
    m_state       = nullptr;
}

Player::~Player()
{
    m_body->GetWorld()->DestroyBody(m_body);
    SDL_DestroyTexture(m_texture);
    delete m_animator;
    delete m_spriteSheet;
    delete m_weapon;
    delete m_state;

    m_body        = nullptr;
    m_animator    = nullptr;
    m_spriteSheet = nullptr;
    m_texture     = nullptr;
}

Player* Player::create(Level* level)
{
    Player* ret = new Player;
    if (ret->init(level))
    {
        return ret;
    }
    delete ret;
    return nullptr;
}

bool Player::init(Level* level)
{
    GameObject::init(GAME_OBJECT_TYPE_PLAYER, level);
    if (!initGraphicsComponent())
    {
        return false;
    }
    initPhysicsComponent();
    setWeapon(new Sword());
    resetMembers();
    return true;
}

bool Player::initGraphicsComponent()
{
    m_texture = IMG_LoadTexture(Locator::getRenderer(), "asserts/player-2.png");
    if (m_texture == nullptr)
    {
        return false;
    }
    m_spriteSheet = new SpriteSheet(m_texture, SPRITE_WIDTH, SPRITE_HEIGHT);
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
    m_timer               = 0.f;
    m_direction           = DIRECTION_RIGHT;
    m_touchingGroundCount = 0;
    m_touchingWallCount   = 0;
    m_maxHitPoints        = 5;
    m_hitPoints           = m_maxHitPoints;
    m_maxManaPoints       = 50;
    m_manaPoints          = m_maxManaPoints;
    if (m_state != nullptr)
    {
        delete m_state;
    }
    m_state           = new PlayerIdle1();
    m_state->m_player = this;
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

    PlayerState* newState = m_state->tick(deltaTime);
    if (newState != nullptr)
    {
        delete m_state;
        m_state           = newState;
        m_state->m_player = this;
        m_state->enter();
    }
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

void Player::getHit(int damage) {}

PlayerState::~PlayerState() {}

PlayerState* PlayerOnGroundState::tick(float deltaTime)
{

    int inputDirection = Input::getInputDirectionX();
    if (Input::isButtonAJustPressed() && Input::isButtonUpPressed())
    {
    }
    else if (Input::isButtonAJustPressed())
    {
		m_player->m_weapon->start();
    }
    else if (Input::isButtonBPressed() && m_player->isOnGround())
    {
        return new PlayerJump();
    }
    else if (inputDirection != 0)
    {
        return new PlayerRun();
    }
    return nullptr;
}

void PlayerIdle1::enter()
{
    m_player->m_animator->play(Player::ANIM_IDLE_1, 0.f);
    m_player->stopHorizontalMovement();
}

void PlayerRun::enter()
{
    m_player->m_animator->play(Player::ANIM_RUN, 0.f);
}

PlayerState* PlayerRun::tick(float deltaTime)
{
    int inputDirection = Input::getInputDirectionX();
    if (inputDirection < 0)
    {
        m_player->m_direction = DIRECTION_LEFT;
    }

    if (inputDirection > 0)
    {
        m_player->m_direction = DIRECTION_RIGHT;
    }

	if (Input::isButtonBPressed() && m_player->isOnGround())
	{
		return new PlayerJump;
	}
    else if (inputDirection != 0)
    {
        b2Vec2 vel = m_player->m_body->GetLinearVelocity();
        vel.x += inputDirection * Player::RUN_ACC;
        vel.x = std::max(-Player::MAX_RUN_SPEED,
                         std::min(Player::MAX_RUN_SPEED, vel.x));
        m_player->m_body->SetLinearVelocity(vel);
    }
    else
    {
        return new PlayerIdle1();
    }
    return nullptr;
}

void PlayerJump::enter()
{
    m_player->m_animator->play(Player::ANIM_JUMP, 0.f);
    m_player->m_body->ApplyLinearImpulseToCenter(b2Vec2(0.f, -Player::JUMP_VEL),
                                                 true);
}

PlayerState* PlayerJump::tick(float deltaTime)
{
    if (m_player->m_animator->isCurrentAnimationFinshed())
    {
        return new PlayerSomersult();
    }
    int inputDirection = Input::getInputDirectionX();
    if (inputDirection < 0)
    {
        m_player->m_direction = DIRECTION_LEFT;
    }

    if (inputDirection > 0)
    {
        m_player->m_direction = DIRECTION_RIGHT;
    }
    if (inputDirection != 0)
    {
        b2Vec2 vel = m_player->m_body->GetLinearVelocity();
        vel.x += inputDirection * Player::RUN_ACC;
        vel.x = std::max(-Player::MAX_RUN_SPEED,
                         std::min(Player::MAX_RUN_SPEED, vel.x));
        m_player->m_body->SetLinearVelocity(vel);
    }
	else 
	{
		m_player->stopHorizontalMovement();
	}
    return nullptr;
}

void PlayerSomersult::enter()
{
	m_player->m_animator->play(Player::ANIM_SOMERSULT, 0.f);
}

PlayerState* PlayerSomersult::tick(float deltaTime)
{
	if (m_player->m_animator->isCurrentAnimationFinshed())
	{
		return new PlayerFall();
	}
    int inputDirection = Input::getInputDirectionX();
    if (inputDirection < 0)
    {
        m_player->m_direction = DIRECTION_LEFT;
    }

    if (inputDirection > 0)
    {
        m_player->m_direction = DIRECTION_RIGHT;
    }
    if (inputDirection != 0)
    {
        b2Vec2 vel = m_player->m_body->GetLinearVelocity();
        vel.x += inputDirection * Player::RUN_ACC;
        vel.x = std::max(-Player::MAX_RUN_SPEED,
                         std::min(Player::MAX_RUN_SPEED, vel.x));
        m_player->m_body->SetLinearVelocity(vel);
    }
	else 
	{
		m_player->stopHorizontalMovement();
	}
	return nullptr;
}

void PlayerFall::enter()
{
	m_player->m_animator->play(Player::ANIM_FALL, 0.f);	
}

PlayerState* PlayerFall::tick(float deltaTime)
{
    int inputDirection = Input::getInputDirectionX();
	if (m_player->isOnGround())
	{
		if (inputDirection != 0)
		{
			return new PlayerRun;
		}
		else 
		{
			return new PlayerIdle1;
		}
	}
    if (inputDirection < 0)
    {
        m_player->m_direction = DIRECTION_LEFT;
    }

    if (inputDirection > 0)
    {
        m_player->m_direction = DIRECTION_RIGHT;
    }
    if (inputDirection != 0)
    {
        b2Vec2 vel = m_player->m_body->GetLinearVelocity();
        vel.x += inputDirection * Player::RUN_ACC;
        vel.x = std::max(-Player::MAX_RUN_SPEED,
                         std::min(Player::MAX_RUN_SPEED, vel.x));
        m_player->m_body->SetLinearVelocity(vel);
    }
	else 
	{
		m_player->stopHorizontalMovement();
	}
	return nullptr;
}
