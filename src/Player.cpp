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
Player::Player()
{
    m_body        = nullptr;
    m_texture     = nullptr;
    m_spriteSheet = nullptr;
    m_animator    = nullptr;
    m_weapon      = nullptr;
    m_spell       = nullptr;
}

Player::~Player()
{
    m_body->GetWorld()->DestroyBody(m_body);
    SDL_DestroyTexture(m_texture);
    delete m_animator;
    delete m_spriteSheet;

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
    anims[ANIM_STAND]         = new Animation(m_spriteSheet, 27, 3, 1.f / 8.f);
    anims[ANIM_CORNER_GRAB]   = new Animation(m_spriteSheet, 30, 4, 1.f / 8.f);
    anims[ANIM_IDLE_2]        = new Animation(m_spriteSheet, 34, 4, 1.f / 10.f);
    anims[ANIM_ATK_1]         = new Animation(m_spriteSheet, 42, 5, 1.f / 12.f);
    anims[ANIM_ATK_2]         = new Animation(m_spriteSheet, 47, 6, 1.f / 12.f);
    anims[ANIM_ATK_3]         = new Animation(m_spriteSheet, 53, 6, 1.f / 12.f);
    anims[ANIM_HURT]          = new Animation(m_spriteSheet, 59, 3, 1.f / 8.f);
    anims[ANIM_DIE]           = new Animation(m_spriteSheet, 61, 7, 1.f / 8.f);
    anims[ANIM_SWORD_DRAW]    = new Animation(m_spriteSheet, 65, 4, 1.f / 8.f);
    anims[ANIM_SWORD_SHEATHE] = new Animation(m_spriteSheet, 69, 4, 1.f / 8.f);
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
    wait();
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
    m_timer += deltaTime;

    if (!isDead() && m_weapon->tick(deltaTime))
    {
        return;
    }
    int inputDirection = Input::getInputDirectionX();
    if (inputDirection < 0)
    {
        m_direction = DIRECTION_LEFT;
    }
    if (inputDirection > 0)
    {
        m_direction = DIRECTION_RIGHT;
    }
    switch (m_state)
    {
    case STATE_IDLE_1:
    {
        if (Input::isButtonAJustPressed() && Input::isButtonUpPressed())
        {
            castSpell();
        }
        else if (Input::isButtonAJustPressed())
        {
            attack();
        }
        else if (Input::isButtonBPressed() && isOnGround())
        {
            jump();
        }
        else if (inputDirection != 0)
        {
            run();
        }
    }
    break;
    case STATE_IDLE_2:
    {
    }
    break;
    case STATE_RUN:
    {
        if (Input::isButtonBPressed() && isOnGround())
        {
            jump();
        }
        else if (inputDirection == 0)
        {
            wait();
        }
        else
        {
            b2Vec2 vel = m_body->GetLinearVelocity();
            vel.x += inputDirection * RUN_ACC;
            vel.x = std::max(-7.f, std::min(7.f, vel.x));
            m_body->SetLinearVelocity(vel);
        }
    }
    break;
    case STATE_JUMP:
    {
        if (m_animator->isCurrentAnimationFinshed())
        {
            somersault();
        }
        else
        {
            if (inputDirection == 0)
            {
                stopHorizontalMovement();
            }
            else
            {
                b2Vec2 vel = m_body->GetLinearVelocity();
                vel.x += inputDirection * RUN_ACC;
                vel.x = std::max(-7.f, std::min(7.f, vel.x));
                m_body->SetLinearVelocity(vel);
            }
        }
    }
    break;
    case STATE_SOMERSULT:
    {
        if (m_animator->isCurrentAnimationFinshed())
        {
            fall();
        }
        else
        {
            if (inputDirection == 0)
            {
                stopHorizontalMovement();
            }
            else
            {
                b2Vec2 vel = m_body->GetLinearVelocity();
                vel.x += inputDirection * RUN_ACC;
                vel.x = std::max(-7.f, std::min(7.f, vel.x));
                m_body->SetLinearVelocity(vel);
            }
        }
    }
    break;
    case STATE_FALL:
    {
        float velX = m_body->GetLinearVelocity().x;
        if (isOnGround())
        {
            if (velX != 0)
            {
                run();
            }
            else
            {
                wait();
            }
        }
        else
        {
            if (inputDirection == 0)
            {
                stopHorizontalMovement();
            }
            else
            {
                b2Vec2 vel = m_body->GetLinearVelocity();
                vel.x += inputDirection * RUN_ACC;
                vel.x = std::max(-7.f, std::min(7.f, vel.x));
                m_body->SetLinearVelocity(vel);
            }
        }
    }
    break;
    case STATE_SWORD_SHEATHE:
    {
    }
    break;
    case STATE_SWORD_DRAW:
    {
    }
    break;
    case STATE_SLIDE:
    {
    }
    break;
    case STATE_STAND:
    {
    }
    break;
    case STATE_HURT:
    {
        if (m_animator->isCurrentAnimationFinshed())
        {
            wait();
        }
    }
    break;
    case STATE_DIE:
    {
		if (m_animator->isCurrentAnimationFinshed())
		{
			resetMembers();
		}
    }
    break;
    }
}

void Player::drawSword()
{
    setState(STATE_SWORD_DRAW, 0.f);
    m_animator->play(ANIM_SWORD_DRAW, 0.f);
}

void Player::sheatheSword()
{
    setState(STATE_SWORD_SHEATHE, 0.f);
    m_animator->play(ANIM_SWORD_SHEATHE, 0.f);
}

void Player::wait()
{
    setState(STATE_IDLE_1, 0.f);
    m_animator->play(ANIM_IDLE_1, 0.f);
    stopHorizontalMovement();
}

void Player::jump()
{
    setState(STATE_JUMP, 0.f);
    m_animator->play(ANIM_JUMP, 0.f);
    m_body->ApplyLinearImpulseToCenter(b2Vec2(0.f, -JUMP_VEL), true);
    m_touchingGroundCount = 0;
}

void Player::run()
{
    setState(STATE_RUN, 0.f);
    m_animator->play(ANIM_RUN, 0.f);
}

void Player::slide()
{
    setState(STATE_SLIDE, 0.f);
    m_animator->play(ANIM_SLIDE, 0.f);
}

void Player::attack() { m_weapon->start(); }

void Player::die()
{
    SDL_Log("Player die");
    setState(STATE_DIE, 0.f);
    m_animator->play(ANIM_DIE, 0.f);
    stopHorizontalMovement();
}

void Player::hurt()
{
    setState(STATE_HURT, 0.f);
    m_animator->play(ANIM_HURT, 0.f);
    stopVerticalMovement();
    m_weapon->cancel();
    int sign = m_direction == DIRECTION_LEFT ? -1 : 1;
    m_body->ApplyLinearImpulseToCenter(b2Vec2(-sign * 2.f, 0.f), true);
}

void Player::somersault()
{
    setState(STATE_SOMERSULT, m_timer);
    m_animator->play(ANIM_SOMERSULT, 0.f);
}

void Player::fall()
{
    setState(STATE_FALL, 0.f);
    m_animator->play(ANIM_FALL, 0.f);
}

void Player::castSpell() { SDL_Log("Player cast spell"); }

void Player::stand()
{
    setState(STATE_STAND, 0.f);
    m_animator->play(ANIM_STAND, 0.f);
}

void Player::getHit(int damage)
{
    SDL_Log("Player get hit");
    if (!isDead() && m_state != STATE_HURT)
    {
        m_hitPoints -= damage;
        if (m_hitPoints < 0)
        {
            m_hitPoints = 0;
        }
        if (m_hitPoints == 0)
        {
            die();
        }
        else
        {
            hurt();
        }
    }
}

bool Player::isDead() const { return m_hitPoints == 0; }

bool Player::isOnGround() const { return m_touchingGroundCount > 0; }

void Player::setState(int newState, float initialTime)
{
    m_state = newState;
    m_timer = initialTime;
}

void Player::touchGround() { ++m_touchingGroundCount; }

void Player::untouchGround()
{
    --m_touchingGroundCount;
    if (m_touchingGroundCount < 0)
    {
        m_touchingGroundCount = 0;
    }
}
