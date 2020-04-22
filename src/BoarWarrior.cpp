#include "BoarWarrior.h"
#include "Animation.h"
#include "Animator.h"
#include "AssertManager.h"
#include "Camera.h"
#include "Constances.h"
#include "Game.h"
#include "Level.h"
#include "Player.h"
#include "SDL_blendmode.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "Utils.h"
#include "Vec.h"
#include "box2d/b2_collision.h"
#include "tmxlite/Object.hpp"

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
    m_texture = textureMGR().getTexture(TextureManager::TEXTURE_BOAR_WARRIOR);
    m_spriteSheet.init(m_texture, 120, 100);
    Animation* anims[NUM_ANIMS];

    anims[ANIM_IDLE]         = new Animation(&m_spriteSheet, 0, 10, 0.2f);
    anims[ANIM_RUN]          = new Animation(&m_spriteSheet, 10, 10, 0.2f);
    anims[ANIM_ATTACK]       = new Animation(&m_spriteSheet, 20, 12, 0.2f);
    anims[ANIM_HEAVY_ATTACK] = new Animation(&m_spriteSheet, 32, 12, 0.2f);

    anims[ANIM_IDLE]->setPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIM_RUN]->setPlayMode(Animation::PLAY_MODE_LOOP);

    m_animator = new Animator(anims, NUM_ANIMS);

    resetMembers();
    return true;
}

void BoarWarrior::takeDamge(int damage)
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
    }
}

void BoarWarrior::onPreSolve(const ContactInfo& info, const b2Manifold&)
{
    if (info.getOtherIdentifier()->tag == TAG_PLAYER)
    {
        Player* player =
            static_cast<Player*>(info.getOtherIdentifier()->object);
        player->takeDamge(1);

        int sign = m_direction == DIRECTION_LEFT ? -1 : 1;
        info.setIsEnabled(false);
    }
}

void BoarWarrior::tick(float deltaTime)
{
    Monster::tick(deltaTime);
    m_animator->tick(deltaTime);
    m_timer += deltaTime;
    m_hurtTimer -= deltaTime;
    m_isHitPlayer = isFacingWithPlayer();
    switch (m_state)
    {
    case STATE_IDLE:
    {
        if (m_timer >= 4.f)
        {
            m_direction = m_direction == DIRECTION_LEFT ? DIRECTION_RIGHT
                                                        : DIRECTION_LEFT;
            m_timer -= 4.f;
            run();
        }
    }
    break;
    case STATE_RUN_FORWARD:
    {
        if (m_timer >= 3.f)
        {
            m_timer -= 3.f;
            idle();
        }
        else
        {
            b2Vec2 vel  = m_body->GetLinearVelocity();
            int    sign = m_direction == DIRECTION_LEFT ? -1 : 1;
            vel.x       = sign * 2.f;
            m_body->SetLinearVelocity(vel);
        }
    }
    break;
    case STATE_RUN_BACKWARD:
    {
    }
    break;
    case STATE_ATTACK:
    {
    }
    break;
    case STATE_HEAVY_ATTACK:
    {
    }
    break;
    case STATE_DIE:
    {
        setNeedToRemove(true);
    }
    break;
    }
}

void BoarWarrior::paint()
{
    const Camera&   camera   = getLayerManager()->getCamera();
    const SDL_Rect& viewport = camera.getViewport();
    SDL_Renderer*   renderer = gameMGR().renderer();
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
        if (m_hurtTimer > 0.f && ((int)(m_timer / 0.02f)) % 2)
        {
			SDL_SetTextureAlphaMod(m_texture, 128);
            sprite.draw(renderer, &dstrect, 0.0, nullptr, flip);
			SDL_SetTextureAlphaMod(m_texture, 255);
        }
        else
        {
            sprite.draw(renderer, &dstrect, 0.0, nullptr, flip);
        }
        if (m_isHitPlayer)
        {
            SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
        }
        int sign = directionToSign(m_direction);
        int len  = sign * Constances::TILE_SIZE * 6;
        int x1   = m_positionX - viewport.x;
        int y1   = m_positionY - viewport.y;
        int x2   = m_positionX + len - viewport.x;
        int y2   = m_positionY - viewport.y;
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }
}

void BoarWarrior::cleanup() { delete this; }

void BoarWarrior::resetMembers()
{
    m_direction = DIRECTION_LEFT;
    m_timer     = 0.f;
    m_hurtTimer = 0.f;
    m_hitPoints = 15.f;
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
	m_body->SetLinearDamping(100.f);
}

void BoarWarrior::run()
{
    m_timer = 0.f;
    m_state = STATE_RUN_FORWARD;
    m_animator->play(ANIM_RUN);
}

void BoarWarrior::attack() {}

void BoarWarrior::heavyAttack() {}

void BoarWarrior::hurt() { m_hurtTimer = 0.06f; }

void BoarWarrior::die() { m_state = STATE_DIE; }

bool BoarWarrior::isFacingWithPlayer()
{
    Player* player     = ((Level*)getLayerManager())->getPlayer();
    b2Body* playerBody = player->getBody();

    float sign = directionToSign(m_direction);

    b2RayCastInput input;
    input.maxFraction = 6;
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
