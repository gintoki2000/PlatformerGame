#include "Player.h"
#include "Animation.h"
#include "Box2D/Box2D.h"
#include "Constances.h"
#include "Game.h"
#include "Level.h"
#include "NTTextureRegion.h"
#include "SDL_error.h"
#include "SDL_image.h"
#include "SDL_keyboard.h"
#include "SDL_render.h"
#include "SDL_scancode.h"

Player::Player() : m_body(nullptr), m_level(nullptr) {}

Player::~Player()
{
    m_level->getWorld()->DestroyBody(m_body);
    SDL_DestroyTexture(m_texture);
}

static std::vector<NTTextureRegion>
createSpriteSheet(SDL_Texture* texture, int startX, int startY, int rows,
                  int cols, int spriteWidth, int spriteHeight)
{
    std::vector<NTTextureRegion> spriteSheet;
    spriteSheet.reserve((unsigned long)(rows * cols));
    SDL_Rect rect{startX, startY, spriteWidth, spriteHeight};
    for (int y = 0; y < rows; ++y)
    {
        for (int x = 0; x < cols; ++x)
        {
            rect.x = x * spriteWidth + startX;
            rect.y = y * spriteHeight + startY;
            rect.w = spriteWidth;
            rect.h = spriteHeight;
            spriteSheet.emplace_back(texture, rect);
        }
    }
    return spriteSheet;
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
    return initialize(level, b2Vec2(Game::WIDTH / 2.f, Game::HEIGHT / 2.f));
}

bool Player::initialize(Level* level, const b2Vec2& position)
{
    if ((m_texture = IMG_LoadTexture(Game::getInstance()->getRenderer(),
                                     "asserts/player.png")) == nullptr)
    {
        SDL_Log("Could not load texture: %s", IMG_GetError());
        return false;
    }
    m_level = level;
    m_timer = 0.f;
    m_state = STATE_IDLE_1;
    m_direction = DIRECTION_LEFT;
    m_isOnGround = false;
    m_continueAttack = false;

    createAnimations();
    createBody(position);
    return true;
}

void Player::update(float dt)
{
    m_isOnGround = false;
    m_isWallSliding = false;
    updateKeyState(dt);
    checkOnGround();

    int inputDirection = 0;
    if (isButtonLeftPressed())
    {
        inputDirection -= 1;
    }
    if (isButtonRightPressed())
    {
        inputDirection += 1;
    }
    if (inputDirection > 0)
    {
        m_direction = DIRECTION_RIGHT;
    }
    if (inputDirection < 0)
    {
        m_direction = DIRECTION_LEFT;
    }
    checkWallSliding();
    m_timer += dt;
    switch (m_state)
    {
    case STATE_IDLE_1:
    {

        if (!m_isOnGround)
        {
            changeState(STATE_FALL);
        }
        else if (isButtonBPressed() && m_isOnGround)
        {
            jump();
        }
        else if (isButtonAJustPressed())
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
        else if (isButtonBPressed() && m_isOnGround)
        {
            jump();
        }
        else if (isButtonAJustPressed())
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
        else if (isButtonBPressed() && m_isOnGround)
        {
            jump();
        }
        else if (isButtonAJustPressed())
        {
            changeState(STATE_ATTACK_1);
            setZeroVelX();
        }
        if (inputDirection != 0)
        {
            move(inputDirection * RUN_ACC * 0.75f);
        }
        else
        {
            setZeroVelX();
            changeState(STATE_IDLE_2);
        }
        break;
    }
    case STATE_JUMP:
    {

        if (m_isWallSliding &&
            ((m_direction == DIRECTION_LEFT && isButtonLeftPressed()) ||
             (m_direction == DIRECTION_RIGHT && isButtonRightPressed())))
        {
            changeState(STATE_WALL_SLIDE);
        }
        else if (isCurrentAnimationComplete())
        {
            changeState(STATE_SMRSLT);
        }

        if (inputDirection != 0)
        {
            move(inputDirection * RUN_ACC * 0.75f);
        }
        else
        {
            setZeroVelX();
        }
        break;
    }
    case STATE_SMRSLT:
    {

        if (isButtonAJustPressed())
        {
            changeState(STATE_AIR_ATTACK_1);
        }

        else if (m_isWallSliding &&
                 ((m_direction == DIRECTION_LEFT && isButtonLeftPressed()) ||
                  (m_direction == DIRECTION_RIGHT && isButtonRightPressed())))
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
            setZeroVelX();
        }
        break;
    }
    case STATE_FALL:
    {

        if (m_isWallSliding &&
            ((m_direction == DIRECTION_LEFT && isButtonLeftPressed()) ||
             (m_direction == DIRECTION_RIGHT && isButtonRightPressed())))
        {
            changeState(STATE_WALL_SLIDE);
        }
        else if (m_isOnGround)
        {
            changeState(STATE_IDLE_2);
        }
        else if (isButtonAJustPressed())
        {
            changeState(STATE_AIR_ATTACK_1);
        }
        if (inputDirection != 0)
        {
            move(inputDirection * RUN_ACC * 0.75f);
        }
        else
        {
            setZeroVelX();
        }
        break;
    }
    case STATE_ATTACK_1:
    {

        if (isButtonAJustPressed())
        {
            m_continueAttack = true;
        }
        if (isCurrentAnimationComplete())
        {
            if (m_continueAttack)
            {
                changeState(STATE_ATTACK_2);
                m_continueAttack = false;
            }
            else
                changeState(STATE_IDLE_2);
            break;
        }
        break;
    }
    case STATE_ATTACK_2:
    {

        if (isButtonAJustPressed())
        {
            m_continueAttack = true;
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

        if (isCurrentAnimationComplete())
        {
            changeState(STATE_IDLE_2);
        }
        break;
    }
    case STATE_AIR_ATTACK_1:
    {

        if (isButtonAJustPressed())
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

        if (isButtonDownPressed() && isButtonAPressed())
        {
            m_continueAttack = true;
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

        if (isButtonBJustPressed())
        {
            float sign = m_direction == DIRECTION_RIGHT ? 1.f : -1.f;
            setZeroVelX();
            m_body->ApplyLinearImpulseToCenter(b2Vec2(sign * -10.f, -15.f),
                                               true);
            changeState(STATE_JUMP);
            m_isWallSliding = false;
        }
        else if (m_isOnGround)
        {
            changeState(STATE_IDLE_2);
        }
        else if (!m_isWallSliding ||
                 (m_direction == DIRECTION_LEFT && !isButtonLeftPressed()) ||
                 (m_direction == DIRECTION_RIGHT && !isButtonRightPressed()))
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
    Animation<NTTextureRegion>& animation = m_animations[m_state];
    const NTTextureRegion& currentFrame = animation.getCurrentFrame(m_timer);
    int drawX = m_body->GetPosition().x * Constances::PPM - SPRITE_WIDTH / 2 -
                viewPort.x;
    int drawY = m_body->GetPosition().y * Constances::PPM - SPRITE_HEIGHT / 2 -
                viewPort.y;
    currentFrame.draw(renderer, drawX, drawY,
                      m_direction == DIRECTION_LEFT ? SDL_FLIP_HORIZONTAL
                                                    : SDL_FLIP_NONE);
}

void Player::changeState(State newState)
{
    if (m_state == newState)
        return;
    m_timer = 0.f;
    m_state = newState;
}

void Player::createBody(const b2Vec2& position)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = true;
    bodyDef.userData = this;
    bodyDef.position.x = position.x / Constances::PPM;
    bodyDef.position.y = position.y / Constances::PPM;
    m_body = m_level->getWorld()->CreateBody(&bodyDef);
    b2PolygonShape shape;
    shape.SetAsBox(WIDTH / 2.f / Constances::PPM,
                   HEIGHT / 2.f / Constances::PPM);
    b2FixtureDef fixtureDef;
    fixtureDef.userData = this;
    fixtureDef.shape = &shape;
    m_fixture = m_body->CreateFixture(&fixtureDef);
}

static std::vector<NTTextureRegion>
createSpriteSheet(SDL_Texture* texture, int startIndex, int cols, int count,
                  int spriteWidth, int spriteHeight)
{
    SDL_Rect r;
    r.w = spriteWidth;
    r.h = spriteHeight;
    std::vector<NTTextureRegion> spriteSheet;
    spriteSheet.reserve(count);
    for (int i = startIndex; i < startIndex + count; ++i)
    {
        int col = i % cols;
        int row = i / cols;
        r.x = col * spriteWidth;
        r.y = row * spriteHeight;
        spriteSheet.emplace_back(texture, r);
    }
    return spriteSheet;
}

void Player::createAnimations()
{
    m_animations[STATE_IDLE_1] = Animation<NTTextureRegion>(
        createSpriteSheet(m_texture, 0, 0, 1, 4, SPRITE_WIDTH, SPRITE_HEIGHT),
        1.f / 6.f, ANIMATION_TYPE_LOOP);
    m_animations[STATE_IDLE_2] = Animation<NTTextureRegion>(
        createSpriteSheet(m_texture, 150, 185, 1, 4, SPRITE_WIDTH,
                          SPRITE_HEIGHT),
        1.f / 8.f, ANIMATION_TYPE_LOOP);
    m_animations[STATE_JUMP] = Animation<NTTextureRegion>(
        createSpriteSheet(m_texture, 0, 74, 1, 4, SPRITE_WIDTH, SPRITE_HEIGHT),
        1.f / 10.f, ANIMATION_TYPE_NORMAL);
    m_animations[STATE_SMRSLT] = Animation<NTTextureRegion>(
        createSpriteSheet(m_texture, 18, 7, 4, SPRITE_WIDTH, SPRITE_HEIGHT),
        1.f / 12.f, ANIMATION_TYPE_NORMAL);
    m_animations[STATE_FALL] = Animation<NTTextureRegion>(
        createSpriteSheet(m_texture, 50, 111, 1, 2, SPRITE_WIDTH,
                          SPRITE_HEIGHT),
        1.f / 10.f, ANIMATION_TYPE_LOOP);
    m_animations[STATE_RUN] = Animation<NTTextureRegion>(
        createSpriteSheet(m_texture, 50, 37, 1, 6, SPRITE_WIDTH, SPRITE_HEIGHT),
        1.f / 10.f, ANIMATION_TYPE_LOOP);
    m_animations[STATE_ATTACK_1] = Animation<NTTextureRegion>(
        createSpriteSheet(m_texture, 0, 222, 1, 5, SPRITE_WIDTH, SPRITE_HEIGHT),
        1.f / 12.f, ANIMATION_TYPE_NORMAL);
    m_animations[STATE_ATTACK_2] = Animation<NTTextureRegion>(
        createSpriteSheet(m_texture, 47, 7, 6, SPRITE_WIDTH, SPRITE_HEIGHT),
        1.f / 12.f, ANIMATION_TYPE_NORMAL);
    m_animations[STATE_ATTACK_3] = Animation<NTTextureRegion>(
        createSpriteSheet(m_texture, 53, 7, 6, SPRITE_WIDTH, SPRITE_HEIGHT),
        1.f / 12.f, ANIMATION_TYPE_NORMAL);
    m_animations[STATE_AIR_ATTACK_1] = Animation<NTTextureRegion>(
        createSpriteSheet(m_texture, 96, 7, 3, SPRITE_WIDTH, SPRITE_HEIGHT),
        1.f / 8.f, ANIMATION_TYPE_NORMAL);
    m_animations[STATE_AIR_ATTACK_2] = Animation<NTTextureRegion>(
        createSpriteSheet(m_texture, 99, 7, 3, SPRITE_WIDTH, SPRITE_HEIGHT),
        1.f / 8.f, ANIMATION_TYPE_NORMAL);
    m_animations[STATE_AIR_ATTACK_3_LOOP] = Animation<NTTextureRegion>(
        createSpriteSheet(m_texture, 103, 7, 2, SPRITE_WIDTH, SPRITE_HEIGHT),
        1.f / 8.f, ANIMATION_TYPE_LOOP);
    m_animations[STATE_AIR_ATTACK_3_RDY] = Animation<NTTextureRegion>(
        createSpriteSheet(m_texture, 105, 7, 1, SPRITE_WIDTH, SPRITE_HEIGHT),
        1.f / 8.f, ANIMATION_TYPE_NORMAL);
    m_animations[STATE_AIR_ATTACK_3_END] = Animation<NTTextureRegion>(
        createSpriteSheet(m_texture, 106, 7, 3, SPRITE_WIDTH, SPRITE_HEIGHT),
        1.f / 8.f, ANIMATION_TYPE_NORMAL);
    m_animations[STATE_WALL_SLIDE] = Animation<NTTextureRegion>(
        createSpriteSheet(m_texture, 79, 7, 2, SPRITE_WIDTH, SPRITE_HEIGHT),
        1.f / 10.f, ANIMATION_TYPE_LOOP);
}

void Player::checkOnGround()
{
    if (m_body->GetLinearVelocity().y < 0.f)
        return;
    b2AABB sensor;
    b2Vec2 halfSize(WIDTH / 2.f / Constances::PPM,
                    HEIGHT / 2.f / Constances::PPM);
    const b2Vec2& pos = m_body->GetPosition();

    sensor.upperBound.x = pos.x - halfSize.x;
    sensor.upperBound.y = pos.y + halfSize.y;

    sensor.lowerBound.x = pos.x + halfSize.x;
    sensor.lowerBound.y = pos.y + halfSize.y + 1.f / Constances::PPM;
    m_checkingStatus = CHECK_GROUND;
    m_level->getWorld()->QueryAABB(this, sensor);
}

void Player::checkWallSliding()
{
    b2AABB sensor;
    const b2Vec2& pos = m_body->GetPosition();
    b2Vec2 halfSize;
    halfSize.x = WIDTH / 2.f / Constances::PPM;
    halfSize.y = HEIGHT / 2.f / Constances::PPM;

    sensor.upperBound.y = pos.y - halfSize.y + 1.f / Constances::PPM;
    sensor.lowerBound.y = pos.y - halfSize.y / 2.f;

    if (m_direction == DIRECTION_LEFT)
    {
        sensor.upperBound.x = pos.x - halfSize.x - 1.f / Constances::PPM;
        sensor.lowerBound.x = pos.x - halfSize.x;
    }
    else
    {
        sensor.upperBound.x = pos.x + halfSize.x;
        sensor.lowerBound.x = pos.x + halfSize.x + 1.f / Constances::PPM;
    }
    m_checkingStatus = CHECK_SLIDE;
    m_level->getWorld()->QueryAABB(this, sensor);
}

bool Player::ReportFixture(b2Fixture* f)
{
    if (f != m_fixture)
    {
        if (m_checkingStatus == CHECK_GROUND)
            m_isOnGround = true;
        else if (m_checkingStatus == CHECK_SLIDE)
            m_isWallSliding = true;
        return false;
    }
    else
        return true;
}

void Player::jump()
{
    m_body->ApplyLinearImpulseToCenter(b2Vec2(0.f, -JUMP_VEL), true);
    changeState(STATE_JUMP);
    m_isOnGround = false;
}

bool Player::isCurrentAnimationComplete()
{
    return m_animations[m_state].isComplete(m_timer);
}

void Player::move(float vx)
{
    m_body->ApplyLinearImpulseToCenter(b2Vec2(vx, 0.f), true);
    b2Vec2 v = m_body->GetLinearVelocity();
    v.x = std::max(-10.f, std::min(10.f, v.x));
    m_body->SetLinearVelocity(v);
}

void Player::setZeroVelX()
{
    b2Vec2 v = m_body->GetLinearVelocity();
    v.x = 0;
    m_body->SetLinearVelocity(v);
}

void Player::updateKeyState(float)
{
    const Uint8* keyState = SDL_GetKeyboardState(nullptr);
    m_prevKeyState = m_currentKeyState;
    m_currentKeyState.buttonA = keyState[SDL_SCANCODE_X];
    m_currentKeyState.buttonB = keyState[SDL_SCANCODE_Z];
    m_currentKeyState.buttonUp = keyState[SDL_SCANCODE_UP];
    m_currentKeyState.buttonDown = keyState[SDL_SCANCODE_DOWN];
    m_currentKeyState.buttonLeft = keyState[SDL_SCANCODE_LEFT];
    m_currentKeyState.buttonRight = keyState[SDL_SCANCODE_RIGHT];
}
