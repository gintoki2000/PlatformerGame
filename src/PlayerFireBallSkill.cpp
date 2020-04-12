#include "PlayerFireBallSkill.h"
#include "Animator.h"
#include "Constances.h"
#include "Enums.h"
#include "Fireball.h"
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
    return false;
}
