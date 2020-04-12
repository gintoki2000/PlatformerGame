#include "PlayerCutSkill.h"
#include "Animator.h"
#include "Input.h"
#include "Player.h"
PlayerCutSkill::PlayerCutSkill() {}

bool PlayerCutSkill::activate(Player& player)
{
    if (player.m_ableToUseSkill &&
        Input::isButtonAJustPressed())
    {
        player.m_horiziontalAcceleration = 0.f;
        player.getAnimator()->pushState(AnimatorState(Player::ANIM_ATK_2));
        return true;
    }
    return false;
}

bool PlayerCutSkill::tick(Player& player, float)
{
    if (player.getAnimator()->isCurrentAnimationFinshed())
    {
        player.getAnimator()->popState();
        return true;
    }
    return false;
}
