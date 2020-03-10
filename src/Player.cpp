#include "Player.h"
#include "Animation.h"
#include "Box2D/Box2D.h"
#include "Constances.h"
#include "Enums.h"
#include "Game.h"
#include "GameObject.h"
#include "Input.h"
#include "Level.h"
#include "Monster.h"
#include "NTTextureRegion.h"
#include "SDL_error.h"
#include "SDL_image.h"
#include "SDL_keyboard.h"
#include "SDL_log.h"
#include "SDL_render.h"
#include "SDL_scancode.h"
Player::AttackingAction::AttackingAction()
{
    player = nullptr;
    level = nullptr;
}
Player::AttackingAction::AttackingAction(Player* _player, Level* _level,
                                         const AttackInfo& _attackInfo)
{
    player = _player;
    level = _level;
    attackInfo = _attackInfo;
}

void Player::AttackingAction::perform()
{

    b2AABB area;
    int sign = player->m_direction == DIRECTION_LEFT ? -1 : 1;
    area.lowerBound.x = player->getPosition().x + attackInfo.dx * (-sign);
    area.lowerBound.y = player->getPosition().y + attackInfo.dy;
    area.upperBound.x = area.lowerBound.x + attackInfo.width;
    area.upperBound.y = area.lowerBound.y + attackInfo.height;
    level->getWorld()->QueryAABB(this, area);
}

bool Player::AttackingAction::ReportFixture(b2Fixture* fixture)
{
    if (fixture->GetBody()->GetUserData() != nullptr)
    {
        GameObject* gameObject = (GameObject*)fixture->GetBody()->GetUserData();
        if (gameObject->getObjectType() == GameObject::Type::MONSTER)
        {
            ((Monster*)gameObject)->getDamage(attackInfo.damage);
        }
    }
    return true;
}

Player::Player() :
    GameObject(GameObject::Type::PLAYER), m_body(nullptr), m_level(nullptr)
{
}

Player::~Player()
{
    m_level->getWorld()->DestroyBody(m_body);
    SDL_DestroyTexture(m_texture);
}

Player* Player::create(Level* level)
{
    Player* ret = new Player;
    if (ret->initialize(level))
        return ret;
    delete ret;
    return nullptr;
}

bool Player::initialize(Level* level)
{
    return initialize(level, b2Vec2(Constances::GAME_WIDTH / 2.f,
                                    Constances::GAME_HEIGHT / 2.f));
}

bool Player::initialize(Level* level, const b2Vec2& position)
{
    if ((m_texture = IMG_LoadTexture(Game::getInstance()->getRenderer(),
                                     "asserts/player.png")) == nullptr)
    {
        SDL_Log("Could not load texture: %s", IMG_GetError());
        return false;
    }

    m_spriteSheet = new SpriteSheet(m_texture, 50, 37);

    m_level = level;
    m_timer = 0.f;
    m_state = STATE_IDLE_1;
    m_direction = DIRECTION_LEFT;
    m_isOnGround = false;
    m_continueAttack = false;
    m_touchingGroundCount = 0;
    m_touchingWallCount = 0;

    AttackInfo attackInfo;
    attackInfo.dx = -WIDTH_IN_METER;
    attackInfo.dy = -HEIGHT_IN_METER / 2.f;
    attackInfo.width = 1.f;
    attackInfo.height = 2.f;
    attackInfo.damage = 1;
    m_attackAction1 = AttackingAction(this, m_level, attackInfo);

    m_attackAction2 = AttackingAction(this, m_level, attackInfo);

    attackInfo.dx = -1.5f;
    attackInfo.dy = 0.f;
    attackInfo.width = 3.f;
    attackInfo.height = 1.f;
    m_attackAction3 = AttackingAction(this, m_level, attackInfo);
    createAnimations();
    createBody(position);
    return true;
}

void Player::update(float dt)
{
    m_isOnGround = isOnGround();
    int inputDirection = Input::getInputDirectionX();

    if (inputDirection < 0)
    {
        m_direction = DIRECTION_LEFT;
    }
    if (inputDirection > 0)
    {
        m_direction = DIRECTION_RIGHT;
    }

    m_isWallSliding = inputDirection != 0 && isTouchingWall();
    m_timer += dt;
    switch (m_state)
    {
    case STATE_IDLE_1:
    {

        if (!m_isOnGround)
        {
            changeState(STATE_FALL);
        }
        else if (Input::isButtonBPressed() && m_isOnGround)
        {
            jump();
        }
        else if (Input::isButtonAJustPressed())
        {
            changeState(STATE_ATTACK_1);
        }
        else if (inputDirection != 0)
        {
            move(inputDirection * RUN_ACC);
            changeState(STATE_RUN);
        }
        break;
    }
    case STATE_IDLE_2:
    {

        if (!m_isOnGround)
        {
            changeState(STATE_FALL);
        }
        else if (Input::isButtonBPressed() && m_isOnGround)
        {
            jump();
        }
        else if (Input::isButtonAJustPressed())
        {
            changeState(STATE_ATTACK_1);
        }
        else if (inputDirection != 0)
        {
            move(inputDirection * RUN_ACC);
            changeState(STATE_RUN);
        }
        else if (m_timer >= 3.f)
        {
            changeState(STATE_IDLE_1);
        }
        break;
    }
    case STATE_RUN:

    {
        if (!m_isOnGround)
        {
            changeState(STATE_FALL);
        }
        else if (Input::isButtonBPressed() && m_isOnGround)
        {
            jump();
        }
        else if (Input::isButtonAJustPressed())
        {
            changeState(STATE_ATTACK_1);
            stopHorizontalMovement();
        }
        if (inputDirection != 0)
        {
            move(inputDirection * RUN_ACC * 0.75f);
        }
        else
        {
            stopHorizontalMovement();
            changeState(STATE_IDLE_2);
        }
        break;
    }
    case STATE_JUMP:
    {

        if (m_isWallSliding &&
            ((m_direction == DIRECTION_LEFT && Input::isButtonLeftPressed()) ||
             (m_direction == DIRECTION_RIGHT && Input::isButtonRightPressed())))
        {
            changeState(STATE_WALL_SLIDE);
        }
        else if (isCurrentAnimationComplete())
        {
            changeState(STATE_SOMMERSAULT);
        }

        if (inputDirection != 0)
        {
            move(inputDirection * RUN_ACC * 0.75f);
        }
        else
        {
            stopHorizontalMovement();
        }
        break;
    }
    case STATE_SOMMERSAULT:
    {

        if (Input::isButtonAJustPressed())
        {
            changeState(STATE_AIR_ATTACK_1);
        }

        else if (m_isWallSliding && ((m_direction == DIRECTION_LEFT &&
                                      Input::isButtonLeftPressed()) ||
                                     (m_direction == DIRECTION_RIGHT &&
                                      Input::isButtonRightPressed())))
        {
            changeState(STATE_WALL_SLIDE);
        }
        else if (isCurrentAnimationComplete())
        {
            changeState(STATE_FALL);
        }
        if (inputDirection != 0)
        {
            move(inputDirection * RUN_ACC * 0.75f);
        }
        else
        {
            stopHorizontalMovement();
        }
        break;
    }
    case STATE_FALL:
    {

        if (m_isWallSliding &&
            ((m_direction == DIRECTION_LEFT && Input::isButtonLeftPressed()) ||
             (m_direction == DIRECTION_RIGHT && Input::isButtonRightPressed())))
        {
            changeState(STATE_WALL_SLIDE);
        }
        else if (m_isOnGround)
        {
            changeState(STATE_IDLE_2);
        }
        else if (Input::isButtonAJustPressed())
        {
            changeState(STATE_AIR_ATTACK_1);
        }
        if (inputDirection != 0)
        {
            move(inputDirection * RUN_ACC * 0.75f);
        }
        else
        {
            stopHorizontalMovement();
        }
        break;
    }
    case STATE_ATTACK_1:
    {

        if (Input::isButtonAJustPressed())
        {
            m_continueAttack = true;
        }

        if (m_animations[m_state].getCurrentIndex(m_timer) == 2)
        {
            m_attackAction1.perform();
        }
        if (isCurrentAnimationComplete())
        {
            if (m_continueAttack)
            {
                changeState(STATE_ATTACK_2);
                m_continueAttack = false;
            }
            else
            {
                changeState(STATE_IDLE_2);
            }
        }
        break;
    }
    case STATE_ATTACK_2:
    {

        if (Input::isButtonAJustPressed())
        {
            m_continueAttack = true;
        }
        if (m_animations[m_state].getCurrentIndex(m_timer) == 2)
        {
            m_attackAction2.perform();
        }
        if (isCurrentAnimationComplete())
        {
            if (m_continueAttack)
            {
                changeState(STATE_ATTACK_3);
                m_continueAttack = false;
            }
            else
                changeState(STATE_IDLE_2);
            break;
        }
        break;
    }
    case STATE_ATTACK_3:
    {

        if (m_animations[m_state].getCurrentIndex(m_timer) == 2)
        {
            m_attackAction3.perform();
        }
        if (isCurrentAnimationComplete())
        {
            changeState(STATE_IDLE_2);
        }
        break;
    }
    case STATE_AIR_ATTACK_1:
    {

        if (Input::isButtonAJustPressed())
        {
            m_continueAttack = true;
        }
        if (isCurrentAnimationComplete())
        {
            if (m_continueAttack)
            {
                m_continueAttack = false;
                changeState(STATE_AIR_ATTACK_2);
            }
            else
            {
                if (m_isOnGround)
                {
                    changeState(STATE_IDLE_2);
                }
                else
                {
                    changeState(STATE_FALL);
                }
            }
        }
        break;
    }
    case STATE_AIR_ATTACK_2:
    {

        if (Input::isButtonDownPressed() && Input::isButtonAPressed())
        {
            m_continueAttack = true;
        }
        if (m_animations[m_state].getCurrentIndex(m_timer) == 2)
        {
        }
        if (isCurrentAnimationComplete())
        {

            if (m_continueAttack)
            {
                m_continueAttack = false;
                m_body->ApplyLinearImpulseToCenter(b2Vec2(0.f, 5.f), true);
                changeState(STATE_AIR_ATTACK_3_LOOP);
            }
            else
            {
                if (m_isOnGround)
                {
                    changeState(STATE_IDLE_2);
                }
                else
                {
                    changeState(STATE_FALL);
                }
            }
        }
        break;
    }
    case STATE_AIR_ATTACK_3_RDY:
    {

        if (isCurrentAnimationComplete())
        {
            changeState(STATE_AIR_ATTACK_3_END);
        }
        break;
    }
    case STATE_AIR_ATTACK_3_LOOP:
    {

        if (m_isOnGround)
        {
            changeState(STATE_AIR_ATTACK_3_RDY);
        }
        break;
    }
    case STATE_AIR_ATTACK_3_END:
    {

        if (isCurrentAnimationComplete())
        {
            changeState(STATE_IDLE_2);
        }
    }
    case STATE_WALL_SLIDE:
    {

        if (Input::isButtonBJustPressed())
        {
            float sign = m_direction == DIRECTION_RIGHT ? 1.f : -1.f;
            m_body->SetLinearVelocity(b2Vec2(sign * -10.f, -15.f));
            changeState(STATE_JUMP);
            m_isWallSliding = false;
        }
        else if (m_isOnGround)
        {
            changeState(STATE_IDLE_2);
        }
        else if (!m_isWallSliding ||
                 (m_direction == DIRECTION_LEFT &&
                  !Input::isButtonLeftPressed()) ||
                 (m_direction == DIRECTION_RIGHT &&
                  !Input::isButtonRightPressed()))
        {
            m_isWallSliding = false;
            changeState(STATE_FALL);
        }
    }
    break;
    }
}

void Player::draw(SDL_Renderer* renderer, const NTRect& viewPort)
{
    /* Animation<NTTextureRegion>& animation = m_animations[m_state];
     const NTTextureRegion& currentFrame = animation.getCurrentFrame(m_timer);
     int drawX = m_body->GetPosition().x * Constances::PPM - SPRITE_WIDTH / 2 -
                 viewPort.x;
     int footY = m_body->GetPosition().y * Constances::PPM + HEIGHT / 2.f;
     int drawY = footY - SPRITE_HEIGHT;
     currentFrame.draw(renderer, drawX, drawY,
                       m_direction == DIRECTION_LEFT ? SDL_FLIP_HORIZONTAL
                                                     : SDL_FLIP_NONE);
         */

    int drawX = m_body->GetPosition().x * Constances::PPM - SPRITE_WIDTH / 2 -
                viewPort.x;
    int footY = m_body->GetPosition().y * Constances::PPM + HEIGHT / 2.f;
    int drawY = footY - SPRITE_HEIGHT;
    int currentFrame = m_animations[m_state].getCurrentFrame(m_timer);
    m_spriteSheet->draw(renderer, currentFrame, drawX, drawY,
                        m_direction == DIRECTION_LEFT ? SDL_FLIP_HORIZONTAL
                                                      : SDL_FLIP_NONE);
}

void Player::changeState(int newState)
{
    if (m_state == newState)
        return;
    m_timer = 0.f;
    m_state = newState;
}

void Player::createBody(const b2Vec2& position)
{
    b2BodyDef bDef;
    bDef.type = b2_dynamicBody;
    bDef.fixedRotation = true;
    bDef.position.x = position.x / Constances::PPM;
    bDef.position.y = position.y / Constances::PPM;
    bDef.userData = this;
    m_body = m_level->getWorld()->CreateBody(&bDef);
    b2PolygonShape shape;
    shape.SetAsBox(WIDTH / 2.f / Constances::PPM,
                   HEIGHT / 2.f / Constances::PPM);

    // main body
    b2FixtureDef fDef;
    fDef.userData = (void*)FIXTURE_TYPE_MAIN_BODY;
    fDef.shape = &shape;
    fDef.filter.categoryBits = CATEGORY_BIT_PLAYER;
    fDef.filter.maskBits = CATEGORY_BIT_MONSTER | CATEGORY_BIT_BLOCK;
    m_fixture = m_body->CreateFixture(&fDef);

    // foot sensor
    fDef = b2FixtureDef();
    shape.SetAsBox((WIDTH / 2.f - 1.f) / Constances::PPM, 2.f / Constances::PPM,
                   b2Vec2(0.f, HEIGHT / 2.f / Constances::PPM), 0.f);
    fDef.isSensor = true;
    fDef.shape = &shape;
    fDef.userData = (void*)FIXTURE_TYPE_FOOT_SENSOR;
    fDef.filter.categoryBits = CATEGORY_BIT_PLAYER;
    fDef.filter.maskBits = CATEGORY_BIT_BLOCK;
    m_body->CreateFixture(&fDef);

    // wall sensor
    shape.SetAsBox(
        1.f / Constances::PPM, HEIGHT / 4.f / Constances::PPM,
        b2Vec2(WIDTH / 2.f / Constances::PPM, -HEIGHT / 4.f / Constances::PPM),
        0.f);
    fDef = b2FixtureDef();
    fDef.shape = &shape;
    fDef.isSensor = true;
    fDef.friction = 0.5f;
    fDef.userData = (void*)FIXTURE_TYPE_RIGHT_WALL_SENSOR;
    fDef.filter.categoryBits = CATEGORY_BIT_PLAYER;
    fDef.filter.maskBits = CATEGORY_BIT_BLOCK;
    m_body->CreateFixture(&fDef);

    // wall sensor
    shape.SetAsBox(
        1.f / Constances::PPM, HEIGHT / 4.f / Constances::PPM,
        b2Vec2(-WIDTH / 2.f / Constances::PPM, -HEIGHT / 4.f / Constances::PPM),
        0.f);
    fDef = b2FixtureDef();
    fDef.shape = &shape;
    fDef.isSensor = true;
    fDef.friction = 0.5f;
    fDef.userData = (void*)FIXTURE_TYPE_RIGHT_WALL_SENSOR;
    fDef.filter.categoryBits = CATEGORY_BIT_PLAYER;
    fDef.filter.maskBits = CATEGORY_BIT_BLOCK;
    m_body->CreateFixture(&fDef);
}
void Player::createAnimations()
{
    m_animations[STATE_IDLE_1] =
        Animation<int>({0, 1, 2, 3}, 1.f / 6.f, ANIMATION_TYPE_LOOP);
    m_animations[STATE_CROUCH] =
        Animation<int>({4, 5, 6, 7}, 1.f / 8.f, ANIMATION_TYPE_LOOP);
    m_animations[STATE_RUN] =
        Animation<int>({8, 9, 10, 11, 12, 13}, 1.f / 8.f, ANIMATION_TYPE_LOOP);
    m_animations[STATE_JUMP] =
        Animation<int>({14, 15, 16, 17}, 1.f / 8.f, ANIMATION_TYPE_NORMAL);
    m_animations[STATE_SOMMERSAULT] =
        Animation<int>({18, 19, 20, 21}, 1.f / 8.f, ANIMATION_TYPE_NORMAL);
    m_animations[STATE_FALL] =
        Animation<int>({22, 23}, 1.f / 8.f, ANIMATION_TYPE_LOOP);
    m_animations[STATE_SLIDE] =
        Animation<int>({24, 25}, 1.f / 8.f, ANIMATION_TYPE_LOOP);
    m_animations[STATE_STAND] =
        Animation<int>({26, 27, 28}, 1.f / 8.f, ANIMATION_TYPE_NORMAL);
    m_animations[STATE_CORNER_GRAB] =
        Animation<int>({29, 30, 31, 32}, 1.f / 8.f, ANIMATION_TYPE_NORMAL);
    m_animations[STATE_CORNER_CLIMB] =
        Animation<int>({33, 34, 35, 36, 37}, 1.f / 8.f, ANIMATION_TYPE_LOOP);
    m_animations[STATE_IDLE_2] =
        Animation<int>({38, 39, 40, 41}, 1.f / 10.f, ANIMATION_TYPE_NORMAL);
    m_animations[STATE_ATTACK_1] =
        Animation<int>({42, 43, 44, 45, 46}, 1.f / 10.f, ANIMATION_TYPE_NORMAL);
    m_animations[STATE_ATTACK_2] = Animation<int>(
        {47, 48, 49, 50, 51, 52}, 1.f / 10.f, ANIMATION_TYPE_NORMAL);
    m_animations[STATE_ATTACK_3] = Animation<int>(
        {53, 54, 55, 56, 57, 58}, 1.f / 10.f, ANIMATION_TYPE_NORMAL);
    m_animations[STATE_DIE] = Animation<int>({59, 60, 61, 62, 63, 64, 65},
                                             1.f / 5.f, ANIMATION_TYPE_NORMAL);
    m_animations[STATE_SWORD_DRAW] =
        Animation<int>({66, 67, 69, 69}, 1.f / 8.f, ANIMATION_TYPE_NORMAL);
    m_animations[STATE_SWORD_SHEATHE] =
        Animation<int>({70, 71, 72, 73}, 1.f / 8.f, ANIMATION_TYPE_NORMAL);
    m_animations[STATE_CORNER_JUMP] =
        Animation<int>({74, 75}, 1.f / 8.f, ANIMATION_TYPE_LOOP);
    m_animations[STATE_WALL_SLIDE] =
        Animation<int>({79, 80}, 1.f / 8.f, ANIMATION_TYPE_LOOP);
    m_animations[STATE_LADDER_CLIMB] =
        Animation<int>({78, 79, 80, 81}, 1.f / 8.f, ANIMATION_TYPE_LOOP);

	m_animator = Animator<int>(m_animations, NUM_STATES);
	
}

void Player::jump()
{
    m_body->ApplyLinearImpulseToCenter(b2Vec2(0.f, -JUMP_VEL), true);
    changeState(STATE_JUMP);
    m_isOnGround = false;
}

void Player::move(float vx)
{
    m_body->ApplyLinearImpulseToCenter(b2Vec2(vx, 0.f), true);
    b2Vec2 vel = m_body->GetLinearVelocity();
    vel.x = std::max(-10.f, std::min(10.f, vel.x));
    m_body->SetLinearVelocity(vel);
}

void Player::stopHorizontalMovement()
{
    b2Vec2 vel = m_body->GetLinearVelocity();
    vel.x = 0.f;
    m_body->SetLinearVelocity(vel);
}

void Player::stopVerticalMovement()
{

    b2Vec2 vel = m_body->GetLinearVelocity();
    vel.y = 0.f;
    m_body->SetLinearVelocity(vel);
}
void Player::touchGround() { ++m_touchingGroundCount; }

void Player::untouchGround() { --m_touchingGroundCount; }

bool Player::isOnGround() const { return m_touchingGroundCount > 0; }

void Player::touchWall() { ++m_touchingWallCount; }

void Player::untouchWall() { --m_touchingWallCount; }

bool Player::isTouchingWall() const { return m_touchingWallCount > 0; }
