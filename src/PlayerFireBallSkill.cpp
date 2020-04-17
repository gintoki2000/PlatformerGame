#include "PlayerFireBallSkill.h"
#include "Animator.h"
#include "Constances.h"
#include "Enums.h"
#include "Input.h"
#include "Level.h"
#include "Player.h"
#include "SDL_mouse.h"
#include "SDL_stdinc.h"

PlayerFireballSkill::PlayerFireballSkill() : m_phrase(0), m_timer(0.f) {}

bool PlayerFireballSkill::activate(Player& player)
{
    if (Input::isButtonAJustPressed() && Input::isButtonUpPressed() && player.m_ableToUseSkill)
    {
        m_phrase = 1;
        m_timer  = 0.f;

        player.getAnimator()->pushState(AnimatorState(Player::ANIM_CAST_SPELL));
        player.m_horiziontalAcceleration = 0.f;

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
            player.getAnimator()->changeState(AnimatorState(Player::ANIM_CAST_LOOP));
            m_phrase = 2;
            m_timer = 0.f;
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
