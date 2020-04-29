#include "BoarWarrior.h"
#include "Animation.h"
#include "ParticleSystem.h"
#include "Animator.h"
#include "AssertManager.h"
#include "Camera.h"
#include "Constances.h"
#include "FireBustParticle.h"
#include "Game.h"
#include "Level.h"
#include "ObjectLayer.h"
#include "Player.h"
#include "Rect.h"
#include "SDL_blendmode.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include "Utils.h"
#include "Vec.h"
#include "box2d/b2_collision.h"
#include "tmxlite/Object.hpp"
#include <cstdlib>

const Vec2 BoarWarrior::SIZE = Vec2(30.f, 48.f);

BoarWarrior::BoarWarrior() { m_animator = nullptr; }

BoarWarrior::~BoarWarrior() { delete m_animator; }

BoarWarrior* BoarWarrior::create(const tmx::Object& data)
{
    BoarWarrior* ret = new BoarWarrior;
    if (ret->init(data))
    {
        return ret;
    }
    DELETE_NULL(ret);
    return nullptr;
}
/*
BoarWarrior* BoarWarrior::create(const Vec2& leftTop)
{
        BoarWarrior* ret = new BoarWarrior;
        if(ret->init(leftTop))
        {
                return ret;
        }
        DELETE_NULL(ret);
        return nullptr;
}
*/
bool BoarWarrior::init(const tmx::Object& data)
{
    Vec2 leftTop;
    leftTop.x = data.getAABB().left;
    leftTop.y = data.getAABB().top;
    return init(leftTop);
}

bool BoarWarrior::init(const Vec2& leftTop)
{
    Monster::init(FloatRect(leftTop, SIZE));
    m_texture = GAME->textureMGR().getTexture(TextureManager::BOAR_WARRIOR);
    m_spriteSheet.init(m_texture, 120, 100);
    Animation* anims[NUM_ANIMS];

    anims[ANIM_IDLE]             = new Animation(&m_spriteSheet, 0, 10, 0.2f);
    anims[ANIM_MOVE_FORWARD]     = new Animation(&m_spriteSheet, 10, 10, 0.2f);
    anims[ANIM_MOVE_BACKWARD]    = new Animation(&m_spriteSheet, 10, 10, 0.2f);
    anims[ANIM_PRE_ATTACK]       = new Animation(&m_spriteSheet, 20, 12, 0.1f);
    anims[ANIM_ATTACK]           = new Animation(&m_spriteSheet, 32, 6, 0.1f);
    anims[ANIM_PRE_HEAVY_ATTACK] = new Animation(&m_spriteSheet, 38, 9, 0.1f);
    anims[ANIM_HEAVY_ATTACK]     = new Animation(&m_spriteSheet, 47, 7, 0.1f);

    anims[ANIM_IDLE]->setPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIM_MOVE_FORWARD]->setPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIM_MOVE_BACKWARD]->setPlayMode(Animation::PLAY_MODE_LOOP_REVERSE);

    m_animator  = new Animator(anims, NUM_ANIMS);
    m_waitTimer = 0.f;

    resetMembers();
    return true;
}

bool BoarWarrior::takeDamge(int damage, Direction direction)
{
    if (m_state != STATE_DIE && m_hurtTimer <= 0.f)
    {
        if (damage > 0)
        {
            m_hitPoints -= damage;
            if (m_hitPoints < 0)
            {
                m_hitPoints = 0;
                die();
            }
            else
            {
                hurt();
            }
        }
        if (direction != DIRECTION_NONE)
        {
            m_direction = direction;
        }
        return true;
    }
    return false;
}

void BoarWarrior::onPreSolve(const ContactInfo& info, const b2Manifold&)
{
    const Identifier* otherIDR = info.getOtherIdentifier();
    if (otherIDR != nullptr && otherIDR->tag == TAG_PLAYER)
    {
        if (!isDead())
        {
            Player*   player = static_cast<Player*>(otherIDR->object);
            Direction direction =
                relativeDirection(player->getPositionX(), getPositionX());
            if (player->takeDamge(1, direction))
            {
                b2Vec2 f;
                f.x = -directionToSign(direction) * 1000.f;
                f.y = -200.f;
                player->getBody()->ApplyForceToCenter(f, true);
            }
        }
        info.setIsEnabled(false);
    }
}

void BoarWarrior::tick(float deltaTime)
{
    Monster::tick(deltaTime);
    m_timer += deltaTime;
    ++m_counter;
    m_animator->tick(deltaTime);
    m_isFacingToPlayer = rayCast(8);

    if (m_hurtTimer > 0)
    {
        --m_hurtTimer;
    }

    if (m_waitTimer > 0.f)
    {
        m_waitTimer -= deltaTime;
        if (m_waitTimer < 0.f)
        {
            m_waitTimer = 0.f;
        }
        return;
    }

    switch (m_state)
    {
    case STATE_IDLE:
    {
        followPlayerDir();
        if (m_isFacingToPlayer)
        {
            moveForward();
        }
    }
    break;
    case STATE_TRIGGER_MOVE_FORWARD:
    {
        followPlayerDir();
        if (!m_isFacingToPlayer)
        {
            idle();
        }
        else if (rayCast(2))
        {
            rand() % 2 == 0 ? preAttack() : preHeavyAttack();
        }
        else
        {
            b2Vec2 vel = m_body->GetLinearVelocity();
            vel.x      = 2.f * directionToSign(m_direction);
            m_body->SetLinearVelocity(vel);
        }
    }
    break;
    case STATE_PRE_ATTACK:
    {
        if (m_phrase == 0)
        {
            if (m_animator->isCurrentAnimationFinshed())
            {
                m_timer  = 0.f;
                m_phrase = 1;
            }
        }
        else if (m_phrase == 1)
        {
            /// wait 1s
            if (m_timer >= 0.5f)
            {
                attack();

                FloatRect rect;
                int       sign = directionToSign(m_direction);

                rect.x      = m_positionX + sign * 32;
                rect.y      = m_positionY - 10;
                rect.width  = 42;
                rect.height = 35;

                if (boxCast(rect, CATEGORY_BIT_PLAYER))
                {
                    Level*  level  = static_cast<Level*>(getLayerManager());
                    Player* player = level->getPlayer();
                    if (player->takeDamge(
                            2, relativeDirection(player->getPositionX(),
                                                 getPositionX())))
                    {
                        player->getBody()->ApplyForceToCenter(
                            b2Vec2(sign * 1000.f, 0.f), true);
                    }
                }
            }
        }
    }
    break;
    case STATE_ATTACK:
    {
        if (m_phrase == 0)
        {
            if (m_animator->isCurrentAnimationFinshed())
            {
                m_timer  = 0.f;
                m_phrase = 1;
            }
        }
        else if (m_phrase == 1)
        {
            if (m_timer > 0.2f)
            {
                moveBackward();
                m_moveBackwardTime = rand() % 10 * 0.1f + 0.5f;
                m_timer            = 0.f;
            }
        }
    }
    break;
    case STATE_TRIGGER_MOVE_BACKWARD:
    {
        followPlayerDir();
        if (!m_isFacingToPlayer)
        {
            idle();
        }
        else if (m_timer >= m_moveBackwardTime)
        {
            idle();
        }
        else
        {

            b2Vec2 vel = m_body->GetLinearVelocity();
            vel.x      = -1.f * directionToSign(m_direction);
            m_body->SetLinearVelocity(vel);
        }
    }
    break;
    case STATE_PRE_HEAVY_ATTACK:
    {

        if (m_phrase == 0)
        {
            if (m_animator->isCurrentAnimationFinshed())
            {
                m_timer  = 0.f;
                m_phrase = 1;
            }
        }
        else if (m_phrase == 1)
        {
            /// wait 1s
            if (m_timer >= 0.6f)
            {
                heavyAttack();
                Level* level = static_cast<Level*>(getLayerManager());

                FloatRect rect;
                int       sign = directionToSign(m_direction);

                rect.x      = m_positionX - 43 / 2 + sign * 32;
                rect.y      = m_positionY - 32;
                rect.width  = 43;
                rect.height = 64;

                if (boxCast(rect, CATEGORY_BIT_PLAYER))
                {
                    Player*   player    = level->getPlayer();
                    Direction direction = relativeDirection(
                        player->getPositionX(), getPositionX());
                    if (player->takeDamge(2, direction))
                    {
                        player->getBody()->ApplyForceToCenter(
                            b2Vec2(0.f, -500.f), true);
                    }
                }
                level->getCameraShaker()->shake(.1f, 5, 15);
            }
        }
    }
    break;
    case STATE_HEAVY_ATTACK:
    {
        if (m_phrase == 0)
        {
            if (m_animator->isCurrentAnimationFinshed())
            {
                m_timer  = 0.f;
                m_phrase = 1;
            }
        }
        else if (m_phrase == 1)
        {
            if (m_timer > 0.3f)
            {
                if (rand() % 2 == 0)
                {
                    m_waitTimer = 2.f;
                    idle();
                }
                else
                {
                    moveBackward();
                }
            }
        }
    }
    break;
    case STATE_DIE:
    {
        if (m_timer <= 1.f)
        {
            if (m_counter % 10 == 0)
            {
                int x = rand() % 70 + m_positionX - 35;
                int y = rand() % 50 + m_positionY - 50;

                Level*            level;
                FireBustParticle* particle;
                level = (Level*)getLayerManager();
                level->getParticleSystem()->create<FireBustParticle>(Vec2(x, y));
            }
            if (m_alpha > 0)
            {

                int d = deltaTime * 255;
                if (d > m_alpha)
                {
                    m_alpha = 0;
                }
                else
                {
                    m_alpha -= d;
                }
            }
        }
        else
        {
            remove();
        }
    }
    break;
    }
}

void BoarWarrior::paint()
{
    const Camera&   camera   = getLayerManager()->getCamera();
    const SDL_Rect& viewport = camera.getViewport();
    SDL_Renderer*   renderer = Game::getInstance()->renderer();
    const Sprite&   sprite   = m_animator->getCurrentSprite();

    SDL_Rect dstrect;
    dstrect.x = m_positionX - 60;
    dstrect.y = m_positionY - 64;
    dstrect.w = sprite.getWidth();
    dstrect.h = sprite.getHeight();
    SDL_RendererFlip flip =
        m_direction == DIRECTION_LEFT ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    if (SDL_HasIntersection(&viewport, &dstrect))
    {
        dstrect.x -= viewport.x;
        dstrect.y -= viewport.y;
        Uint8 a;
        SDL_GetTextureAlphaMod(m_texture, &a);
        SDL_SetTextureAlphaMod(m_texture, m_alpha);
        if (m_hurtTimer > 0 && (m_hurtTimer / 5) % 2 == 0)
        {
            Uint8 r, g, b;
            SDL_GetTextureColorMod(m_texture, &r, &g, &b);
            SDL_SetTextureColorMod(m_texture, 0xff, 0x00, 0x0);
            sprite.draw(renderer, &dstrect, 0.0, nullptr, flip);
            SDL_SetTextureColorMod(m_texture, r, g, b);
        }
        else
        {
            sprite.draw(renderer, &dstrect, 0.0, nullptr, flip);
        }
        SDL_SetTextureAlphaMod(m_texture, a);
    }
}

void BoarWarrior::cleanup() { delete this; }

void BoarWarrior::resetMembers()
{
    m_direction = DIRECTION_LEFT;
    m_timer     = 0.f;
    m_hurtTimer = 0.f;
    m_hitPoints = 50;
    m_alpha     = 255;
    idle();
}

void BoarWarrior::idle()
{
    m_timer = 0.f;
    m_state = STATE_IDLE;
    m_animator->play(ANIM_IDLE);
    b2Vec2 vel = m_body->GetLinearVelocity();
    vel.x      = 0.f;
    m_body->SetLinearVelocity(vel);
}

void BoarWarrior::moveForward()
{
    m_state = STATE_TRIGGER_MOVE_FORWARD;
    m_animator->play(ANIM_MOVE_FORWARD);
}

void BoarWarrior::moveBackward()
{
    m_state = STATE_TRIGGER_MOVE_BACKWARD;
    m_animator->play(ANIM_MOVE_BACKWARD);
    m_phrase = 0;
    m_timer  = 0.f;
}

void BoarWarrior::preAttack()
{
    m_state = STATE_PRE_ATTACK;
    m_animator->play(ANIM_PRE_ATTACK);
    b2Vec2 vel = m_body->GetLinearVelocity();
    vel.x      = 0.f;
    m_body->SetLinearVelocity(vel);
    m_phrase = 0;
}

void BoarWarrior::attack()
{
    m_state = STATE_ATTACK;
    m_animator->play(ANIM_ATTACK);
    m_phrase = 0;
}

void BoarWarrior::preHeavyAttack()
{

    m_state = STATE_PRE_HEAVY_ATTACK;
    m_animator->play(ANIM_PRE_HEAVY_ATTACK);
    b2Vec2 vel = m_body->GetLinearVelocity();
    vel.x      = 0.f;
    m_body->SetLinearVelocity(vel);
    m_phrase = 0;
}
void BoarWarrior::heavyAttack()
{
    m_state = STATE_HEAVY_ATTACK;
    m_animator->play(ANIM_HEAVY_ATTACK);
    m_phrase = 0;
}

void BoarWarrior::hurt() { m_hurtTimer = 17; }

void BoarWarrior::die()
{
    m_state     = STATE_DIE;
    m_counter   = 0;
    m_timer     = 0.f;
    m_waitTimer = 0.f;
    m_animator->play(ANIM_IDLE);
    m_body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
    m_body->SetGravityScale(0.f);
}

void BoarWarrior::wait() { m_state = STATE_WAIT; }

bool BoarWarrior::rayCast(int dis)
{
    Player* player     = ((Level*)getLayerManager())->getPlayer();
    b2Body* playerBody = player->getBody();

    float sign = directionToSign(m_direction);

    b2RayCastInput input;
    input.maxFraction = dis;
    input.p1.x        = m_body->GetPosition().x;
    input.p1.y        = m_body->GetPosition().y;
    input.p2.x        = input.p1.x + sign * 1.f;
    input.p2.y        = input.p1.y;

    b2RayCastOutput output;

    for (b2Fixture* f = playerBody->GetFixtureList(); f != nullptr;
         f            = f->GetNext())
    {
        if (f->RayCast(&output, input, 0))
        {
            return true;
        }
    }
    return false;
}

void BoarWarrior::followPlayerDir()
{

    Player* player     = ((Level*)getLayerManager())->getPlayer();
    float   playerPosX = player->getPositionX();

    if (playerPosX < m_positionX)
    {
        m_direction = DIRECTION_LEFT;
    }
    if (playerPosX > m_positionX)
    {
        m_direction = DIRECTION_RIGHT;
    }
}
