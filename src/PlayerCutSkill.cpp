#include "PlayerCutSkill.h"
#include "Animator.h"
#include "Input.h"
#include "Player.h"
PlayerCutSkill::PlayerCutSkill() {}

bool PlayerCutSkill::activate(Player& player)
{
    if (player.getStatus() == Player::STATUS_NORMAL &&
        Input::isButtonAJustPressed())
    {
        player.stopHorizontalMovement();
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
