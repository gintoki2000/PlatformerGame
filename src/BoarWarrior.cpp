#include "BoarWarrior.h"
#include "Animation.h"
#include "ParticleSystem.h"
#include "Animator.h"

#include "Camera.h"
#include "Constances.h"
#include "FireBustParticle.h"
#include "Game.h"
#include "Level.h"
#include "ObjectLayer.h"
#include "Adventurer.h"
#include "Rect.h"
#include "SDL_blendmode.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include "TextureManager.h"
#include "Utils.h"
#include "Vec.h"
#include "box2d/b2_collision.h"
#include "tmxlite/Object.hpp"
#include <cstdlib>

const Vec2 BoarWarrior::SIZE = Vec2(30.f, 48.f);

BoarWarrior::BoarWarrior() { m_animator = nullptr; }

BoarWarrior::~BoarWarrior() { delete m_animator; }

BoarWarrior* BoarWarrior::Create(const tmx::Object& data)
{
    BoarWarrior* ret = new BoarWarrior;
    if (ret->Init(data))
    {
        return ret;
    }
    DELETE_NULL(ret);
    return nullptr;
}
/*
BoarWarrior* BoarWarrior::Create(const Vec2& leftTop)
{
        BoarWarrior* ret = new BoarWarrior;
        If(ret->Init(leftTop))
        {
                return ret;
        }
        DELETE_NULL(ret);
        return nullptr;
}
*/
bool BoarWarrior::Init(const tmx::Object& data)
{
    Vec2 leftTop;
    leftTop.x = data.getAABB().left;
    leftTop.y = data.getAABB().top;
    return Init(leftTop);
}

bool BoarWarrior::Init(const Vec2& leftTop)
{
    Monster::Init(FloatRect(leftTop, SIZE));
    m_texture = TextureManager::Get(TEX_BOAR_WARRIOR);
    m_spriteSheet.Init(m_texture, 120, 100);
    Animation* anims[NUM_ANIMS];

    anims[ANIm_IDLE]             = new Animation(&m_spriteSheet, 0, 10, 0.2f);
    anims[ANIm_MOVE_FORWARD]     = new Animation(&m_spriteSheet, 10, 10, 0.2f);
    anims[ANIm_MOVE_BACKWARD]    = new Animation(&m_spriteSheet, 10, 10, 0.2f);
    anims[ANIm_PRE_ATTACK]       = new Animation(&m_spriteSheet, 20, 12, 0.1f);
    anims[ANIm_ATTACK]           = new Animation(&m_spriteSheet, 32, 6, 0.1f);
    anims[ANIm_PRE_HEAVY_ATTACK] = new Animation(&m_spriteSheet, 38, 9, 0.1f);
    anims[ANIm_HEAVY_ATTACK]     = new Animation(&m_spriteSheet, 47, 7, 0.1f);

    anims[ANIm_IDLE]->SetPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIm_MOVE_FORWARD]->SetPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIm_MOVE_BACKWARD]->SetPlayMode(Animation::PLAY_MODE_LOOP_REVERSE);

    m_animator  = new Animator(anims, NUM_ANIMS);
    m_waitTimer = 0.f;

    ResetMembers();
    return true;
}

bool BoarWarrior::TakeDamge(int damage, Direction direction)
{
    if (m_state != STATE_DIE && m_hurtTimer <= 0.f)
    {
        if (damage > 0)
        {
            m_hitPoints -= damage;
            if (m_hitPoints < 0)
            {
                m_hitPoints = 0;
                Die();
            }
            else
            {
                Hurt();
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

void BoarWarrior::OnPreSolve(const ContactInfo& info, const b2Manifold&)
{
    const Identifier* otherIDR = info.GetOtherIdentifier();
    if (otherIDR != nullptr && otherIDR->tag == TAG_PLAYER)
    {
        if (!IsDead())
        {
            Adventurer*   adventurer = static_cast<Adventurer*>(otherIDR->object);
            Direction direction =
                RelativeDirection(adventurer->GetPositionX(), GetPositionX());
            if (adventurer->TakeDamge(1, direction) && adventurer->IsGrounded())
            {
                b2Vec2 f;
                f.x = -DirectionToSign(direction) * 20.f;
                f.y = -10.f;
                adventurer->GetBody()->ApplyLinearImpulseToCenter(f, true);
            }
        }
        info.SetIsEnabled(false);
    }
}

void BoarWarrior::Tick(float deltaTime)
{
    Monster::Tick(deltaTime);
    m_timer += deltaTime;
    ++m_counter;
    m_animator->Tick(deltaTime);
    m_isFacingToAdventurer = RayCast(8);

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
        FollowAdventurerDir();
        if (m_isFacingToAdventurer)
        {
            MoveForward();
        }
    }
    break;
    case STATE_TRIGGER_MOVE_FORWARD:
    {
        FollowAdventurerDir();
        if (!m_isFacingToAdventurer)
        {
            Idle();
        }
        else if (RayCast(2))
        {
            rand() % 2 == 0 ? PreAttack() : PreHeavyAttack();
        }
        else
        {
            b2Vec2 vel = m_body->GetLinearVelocity();
            vel.x      = 2.f * DirectionToSign(m_direction);
            m_body->SetLinearVelocity(vel);
        }
    }
    break;
    case STATE_PRE_ATTACK:
    {
        if (m_phrase == 0)
        {
            if (m_animator->IsCurrentAnimationFinshed())
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
                Attack();

                FloatRect rect;
                int       sign = DirectionToSign(m_direction);

                rect.x      = m_positionX + sign * 32;
                rect.y      = m_positionY - 10;
                rect.width  = 42;
                rect.height = 35;

                if (BoxCast(rect, CATEGORY_BIT_PLAYER))
                {
                    Level*  level  = static_cast<Level*>(GetScene());
                    Adventurer* adventurer = level->GetAdventurer();
                    if (adventurer->TakeDamge(
                            2, RelativeDirection(adventurer->GetPositionX(),
                                                 GetPositionX())))
                    {
                        adventurer->GetBody()->ApplyForceToCenter(
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
            if (m_animator->IsCurrentAnimationFinshed())
            {
                m_timer  = 0.f;
                m_phrase = 1;
            }
        }
        else if (m_phrase == 1)
        {
            if (m_timer > 0.2f)
            {
                MoveBackward();
                m_moveBackwardTime = rand() % 10 * 0.1f + 0.5f;
                m_timer            = 0.f;
            }
        }
    }
    break;
    case STATE_TRIGGER_MOVE_BACKWARD:
    {
        FollowAdventurerDir();
        if (!m_isFacingToAdventurer)
        {
            Idle();
        }
        else if (m_timer >= m_moveBackwardTime)
        {
            Idle();
        }
        else
        {

            b2Vec2 vel = m_body->GetLinearVelocity();
            vel.x      = -1.f * DirectionToSign(m_direction);
            m_body->SetLinearVelocity(vel);
        }
    }
    break;
    case STATE_PRE_HEAVY_ATTACK:
    {

        if (m_phrase == 0)
        {
            if (m_animator->IsCurrentAnimationFinshed())
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
                HeavyAttack();
                Level* level = static_cast<Level*>(GetScene());

                FloatRect rect;
                int       sign = DirectionToSign(m_direction);

                rect.x      = m_positionX - 43 / 2 + sign * 32;
                rect.y      = m_positionY - 32;
                rect.width  = 43;
                rect.height = 64;

                if (BoxCast(rect, CATEGORY_BIT_PLAYER))
                {
                    Adventurer*   adventurer    = level->GetAdventurer();
                    Direction direction = RelativeDirection(
                        adventurer->GetPositionX(), GetPositionX());
                    if (adventurer->TakeDamge(2, direction))
                    {
                        adventurer->GetBody()->ApplyForceToCenter(
                            b2Vec2(0.f, -500.f), true);
                    }
                }
                level->GetCameraShaker()->Shake(.1f, 5, 15);
            }
        }
    }
    break;
    case STATE_HEAVY_ATTACK:
    {
        if (m_phrase == 0)
        {
            if (m_animator->IsCurrentAnimationFinshed())
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
                    Idle();
                }
                else
                {
                    MoveBackward();
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
                level = (Level*)GetScene();
                level->GetParticleSystem()->Create<FireBustParticle>(Vec2(x, y));
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
            Remove();
        }
    }
    break;
    }
}

void BoarWarrior::Paint()
{
    const Camera&   camera   = GetScene()->GetCamera();
    const SDL_Rect& viewport = camera.GetViewport();
    SDL_Renderer*   renderer = Game::GetInstance()->GetRenderer();
    const Sprite&   sprite   = m_animator->GetCurrentSprite();

    SDL_Rect dstrect;
    dstrect.x = m_positionX - 60;
    dstrect.y = m_positionY - 64;
    dstrect.w = sprite.GetWidth();
    dstrect.h = sprite.GetHeight();
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
            sprite.Draw(renderer, &dstrect, 0.0, nullptr, flip);
            SDL_SetTextureColorMod(m_texture, r, g, b);
        }
        else
        {
            sprite.Draw(renderer, &dstrect, 0.0, nullptr, flip);
        }
        SDL_SetTextureAlphaMod(m_texture, a);
    }
}

void BoarWarrior::Cleanup() { delete this; }

void BoarWarrior::ResetMembers()
{
    m_direction = DIRECTION_LEFT;
    m_timer     = 0.f;
    m_hurtTimer = 0.f;
    m_hitPoints = 50;
    m_alpha     = 255;
    Idle();
}

void BoarWarrior::Idle()
{
    m_timer = 0.f;
    m_state = STATE_IDLE;
    m_animator->Play(ANIm_IDLE);
    b2Vec2 vel = m_body->GetLinearVelocity();
    vel.x      = 0.f;
    m_body->SetLinearVelocity(vel);
}

void BoarWarrior::MoveForward()
{
    m_state = STATE_TRIGGER_MOVE_FORWARD;
    m_animator->Play(ANIm_MOVE_FORWARD);
}

void BoarWarrior::MoveBackward()
{
    m_state = STATE_TRIGGER_MOVE_BACKWARD;
    m_animator->Play(ANIm_MOVE_BACKWARD);
    m_phrase = 0;
    m_timer  = 0.f;
}

void BoarWarrior::PreAttack()
{
    m_state = STATE_PRE_ATTACK;
    m_animator->Play(ANIm_PRE_ATTACK);
    b2Vec2 vel = m_body->GetLinearVelocity();
    vel.x      = 0.f;
    m_body->SetLinearVelocity(vel);
    m_phrase = 0;
}

void BoarWarrior::Attack()
{
    m_state = STATE_ATTACK;
    m_animator->Play(ANIm_ATTACK);
    m_phrase = 0;
}

void BoarWarrior::PreHeavyAttack()
{

    m_state = STATE_PRE_HEAVY_ATTACK;
    m_animator->Play(ANIm_PRE_HEAVY_ATTACK);
    b2Vec2 vel = m_body->GetLinearVelocity();
    vel.x      = 0.f;
    m_body->SetLinearVelocity(vel);
    m_phrase = 0;
}
void BoarWarrior::HeavyAttack()
{
    m_state = STATE_HEAVY_ATTACK;
    m_animator->Play(ANIm_HEAVY_ATTACK);
    m_phrase = 0;
}

void BoarWarrior::Hurt() { m_hurtTimer = 17; }

void BoarWarrior::Die()
{
    m_state     = STATE_DIE;
    m_counter   = 0;
    m_timer     = 0.f;
    m_waitTimer = 0.f;
    m_animator->Play(ANIm_IDLE);
    m_body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
    m_body->SetGravityScale(0.f);
}

void BoarWarrior::Wait() { m_state = STATE_WAIT; }

bool BoarWarrior::RayCast(int dis)
{
    Adventurer* adventurer     = ((Level*)GetScene())->GetAdventurer();
    b2Body* adventurerBody = adventurer->GetBody();

    float sign = DirectionToSign(m_direction);

    b2RayCastInput input;
    input.maxFraction = dis;
    input.p1.x        = m_body->GetPosition().x;
    input.p1.y        = m_body->GetPosition().y;
    input.p2.x        = input.p1.x + sign * 1.f;
    input.p2.y        = input.p1.y;

    b2RayCastOutput output;

    for (b2Fixture* f = adventurerBody->GetFixtureList(); f != nullptr;
         f            = f->GetNext())
    {
        if (f->RayCast(&output, input, 0))
        {
            return true;
        }
    }
    return false;
}

void BoarWarrior::FollowAdventurerDir()
{

    Adventurer* adventurer     = ((Level*)GetScene())->GetAdventurer();
    float   adventurerPosX = adventurer->GetPositionX();

    if (adventurerPosX < m_positionX)
    {
        m_direction = DIRECTION_LEFT;
    }
    if (adventurerPosX > m_positionX)
    {
        m_direction = DIRECTION_RIGHT;
    }
}
