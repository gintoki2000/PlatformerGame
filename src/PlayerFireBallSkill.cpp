#include "PlayerFireBallSkill.h"
#include "Animator.h"
#include "Constances.h"
#include "FireCastParticle.h"
#include "Fireball.h"
#include "Input.h"
#include "Level.h"
#include "ObjectLayer.h"
#include "Player.h"
#include "SDL_mouse.h"
#include "SDL_stdinc.h"
#include "Utils.h"
#include "Vec.h"

PlayerFireballSkill::PlayerFireballSkill() :
    m_phrase(0), m_timer(0.f), m_particle(new FireCastParticle())
{
}

PlayerFireballSkill::~PlayerFireballSkill() { DELETE_NULL(m_particle); }

bool PlayerFireballSkill::activate(Player& player)
{
    if (Input::isButtonAJustPressed() && Input::isButtonUpPressed() &&
        player.m_ableToUseSkill && m_particle->isInUse() == false)
    {
        m_phrase = 1;
        m_timer  = 0.f;

        player.getAnimator()->pushState(AnimatorState(Player::ANIM_CAST_SPELL));
        player.m_horiziontalAcceleration = 0.f;

        float sign = player.m_direction == DIRECTION_LEFT ? -1.f : 1.f;

        Vec2 pos;
        pos.x = player.getPositionX() + 16.f * sign;
        pos.y = player.getPositionY();
        m_particle->init(pos);

        FireBall* fireball = FireBall::create(pos, player.m_direction, 10.f);

        Level* level = (Level*)player.getLayerManager();

        level->getParticleLayer()->addObject(m_particle);
        level->getParticleLayer()->addObject(fireball);

        return true;
    }
    return false;
}

bool PlayerFireballSkill::tick(Player& player, float deltaTime)
{
    if (m_phrase == 1)
    {
        if (player.getAnimator()->isCurrentAnimationFinshed())
        {
            player.getAnimator()->changeState(
                AnimatorState(Player::ANIM_CAST_LOOP));
            m_phrase = 2;
            m_timer  = 0.f;
        }
    }
    else if (m_phrase == 2)
    {
        m_timer += deltaTime;
        if (m_timer >= .5f)
        {
            player.getAnimator()->popState();
            return true;
        }
    }
    if (!player.isGrounded())
    {
        b2Vec2 vel = player.getBody()->GetLinearVelocity();
        if (Input::isButtonBReleased() && vel.y < 0.f)
        {
            vel.y *= player.m_cutJumpHeight;
            player.getBody()->SetLinearVelocity(vel);
        }

        int inputDirection = Input::getHorizontalInputDirection();
        if (inputDirection != 0)
        {
            player.m_horiziontalAcceleration = player.m_runAcceleration;
        }
        else
        {
            player.m_horiziontalAcceleration = 0.f;
        }
    }
    return false;
}
