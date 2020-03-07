#include "Slime.h"
#include "Animation.h"
#include "Constances.h"
#include "Enums.h"
#include "Game.h"
#include "Level.h"
#include "Player.h"
#include "SDL_image.h"
#include "SDL_render.h"
#include "SpriteSheet.h"

Slime::Asserts::Asserts() : texture(nullptr), spriteSheet(nullptr) {}

Slime::Asserts::~Asserts()
{
    delete spriteSheet;
    SDL_DestroyTexture(texture);
}

bool Slime::Asserts::load()
{
    if ((texture = IMG_LoadTexture(Game::getInstance()->getRenderer(),
                                   "asserts/slime.png")) == nullptr)
    {
        SDL_Log("Failed to load asserts/slime.png: %s", IMG_GetError());
        return false;
    }

    spriteSheet = new SpriteSheet(texture, 32, 25);
    return true;
}

Slime::Slime() : Monster(Monster::Type::SMLIE), m_pool(nullptr) {}

Slime::~Slime() {}

bool Slime::create(Pool* pool, const Asserts& asserts)
{
    m_pool = pool;
    m_asserts = &asserts;

    m_animations[STATE_IDLE] =
        Animation<int>({0, 1, 2, 3}, 1.f / 5.f, ANIMATION_TYPE_LOOP);
    m_animations[STATE_MOVE] =
        Animation<int>({4, 5, 6, 7}, 1.f / 6.f, ANIMATION_TYPE_LOOP);
    m_animations[STATE_ATTACK] =
        Animation<int>({8, 9, 10, 11, 12}, 1.f / 6.f, ANIMATION_TYPE_NORMAL);
    m_animations[STATE_HURT] =
        Animation<int>({13, 14, 15}, 1.f / 5.f, ANIMATION_TYPE_NORMAL);
    m_animations[STATE_DIE] =
        Animation<int>({16, 17, 18, 19, 20}, 1.f / 4.f, ANIMATION_TYPE_NORMAL);

    return true;
}

void Slime::createBody(b2World* world, const b2Vec2& position)
{
    b2BodyDef bDef;
    bDef.fixedRotation = true;
    bDef.position = position;
    bDef.type = b2_dynamicBody;
    bDef.userData = this;
    m_body = world->CreateBody(&bDef);

    b2PolygonShape shape;
    shape.SetAsBox(WIDTH / 2.f / Constances::PPM,
                   HEIGHT / 2.f / Constances::PPM);
    b2FixtureDef fDef;
    fDef.shape = &shape;
    fDef.restitution = 0.f;
    fDef.friction = 0.f;
    fDef.filter.categoryBits = CATEGORY_BIT_MONSTER;
    fDef.filter.maskBits = CATEGORY_BIT_BLOCK;
    m_body->CreateFixture(&fDef);
}

Slime* Slime::initialize(Level* level, const b2Vec2& position)
{
    m_timer = 0.f;
    m_state = STATE_IDLE;
    m_level = level;
    m_direction = DIRECTION_LEFT;
    m_directionTimer = 2.f;
    m_hitPoints = 10;
    createBody(level->getWorld(), position);
    return this;
}

void Slime::update(float dt)
{
    m_timer += dt;
    m_directionTimer += dt;
    if (m_directionTimer >= 2.f)
    {
        m_direction = getFacingPlayerDirection();
        m_directionTimer = 0.f;
    }
    switch (m_state)
    {
    case STATE_IDLE:
    {
        if (m_timer < 1.5f)
            return;
        if (getDistanceToPlayer() < 7.f)
        {
            int sign = m_direction == DIRECTION_LEFT ? -1.f : 1.f;
            setHorizontalVel(sign * 1.f);
            m_state = STATE_MOVE;
            m_timer = 0.f;
        }
        break;
    }
    case STATE_MOVE:
    {
        float distanceToPlayer = getDistanceToPlayer();
        float sign = m_direction == DIRECTION_LEFT ? -1.f : 1.f;
        if (distanceToPlayer < 2.f)
        {
            m_state = STATE_ATTACK;
            m_timer = 0.f;
        }
        else if (distanceToPlayer > 7.f)
        {
            setHorizontalVel(0.f);
            m_state = STATE_IDLE;
            m_timer = 0.f;
        }
        else
        {
            setHorizontalVel(sign * 1.f);
        }
        break;
    }
    case STATE_ATTACK:
    {
        if (m_animations[m_state].isComplete(m_timer))
        {
            m_state = STATE_IDLE;
            m_timer = 0.f;
            setHorizontalVel(0.f);
        }
        else if (getDistanceToPlayer() > 1.f)
        {
            float sign = m_direction == DIRECTION_LEFT ? -1.f : 1.f;
            setHorizontalVel(sign * 2.f);
        }
        else
        {
            setHorizontalVel(0.f);
        }
        break;
    }
    case STATE_HURT:
    {
        if (m_animations[m_state].isComplete(m_timer))
        {
            m_state = STATE_IDLE;
            m_timer = 0.f;
            setHorizontalVel(0.f);
        }
        break;
    }
    case STATE_DIE:
    {
        if (m_animations[m_state].isComplete(m_timer))
        {
            m_level->removeMonster(this);
        }
        break;
    }
    }
}

void Slime::draw(SDL_Renderer* renderer, const NTRect&)
{
    int spriteIndex = m_animations[m_state].getCurrentFrame(m_timer);
    const Sprite& sprite = m_asserts->spriteSheet->getSprite(spriteIndex);
    int drawX =
        m_body->GetPosition().x * Constances::PPM - Asserts::SPRITE_WIDTH / 2.f;
    int footY = m_body->GetPosition().y * Constances::PPM + HEIGHT / 2.f;
    int drawY = footY - Asserts::SPRITE_HEIGHT;

    sprite.draw(renderer, drawX, drawY,
                m_direction == DIRECTION_LEFT ? SDL_FLIP_NONE
                                              : SDL_FLIP_HORIZONTAL);
}

void Slime::cleanup()
{
    m_level->getWorld()->DestroyBody(m_body);
    m_body = nullptr;
    m_level = nullptr;
    m_pool->releaseObject(this);
}

float Slime::getDistanceToPlayer()
{
    b2Vec2 diff = m_body->GetPosition() - m_level->getPlayer()->getPosition();
    return diff.Length();
}

Direction Slime::getFacingPlayerDirection()
{
    float playerX = m_level->getPlayer()->getPosition().x;
    float monsterX = m_body->GetPosition().x;
    if (playerX - monsterX < 0)
    {
        return DIRECTION_LEFT;
    }
    else
    {
        return DIRECTION_RIGHT;
    }
}

void Slime::setHorizontalVel(float v)
{
    b2Vec2 vel = m_body->GetLinearVelocity();
    vel.x = v;
    m_body->SetLinearVelocity(vel);
}

void Slime::getDamage(int damage)
{
    if (m_state != STATE_DIE && m_state != STATE_HURT)
    {
        m_hitPoints -= damage;
        if (m_hitPoints <= 0)
        {
            m_state = STATE_DIE;
            m_timer = 0.f;
			setHorizontalVel(0.f);
        }
        else
        {
            m_timer = 0.f;
            m_state = STATE_HURT;
            float sign = m_direction == DIRECTION_LEFT ? -1.f : 1.f;
            m_body->SetLinearVelocity(b2Vec2(-sign * 1.5f, -2.f));
        }
    }
}
