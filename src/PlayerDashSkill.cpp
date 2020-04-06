#include "PlayerDashSkill.h"
#include "Animator.h"
#include "Enums.h"
#include "Input.h"
#include "Player.h"

PlayerDashSkill::PlayerDashSkill() { m_maxDashTimes = 2; }

bool PlayerDashSkill::activate(Player& player)
{
    if (!player.wasGrounded() && player.isGrounded())
    {
        m_leftTimes = m_maxDashTimes;
    }
    if (!player.isGrounded() && m_leftTimes > 0 &&
        player.getStatus() == Player::STATUS_NORMAL &&
        Input::isButtonUpPressed() && Input::isButtonBJustPressed())
    {
        player.getBody()->SetGravityScale(0.f);
        player.getAnimator()->pushState(AnimatorState(Player::ANIM_DASH));
        b2Vec2 vel;
        vel.y = 0.f;
        vel.x = player.getDirection() == DIRECTION_LEFT ? -20.f : 20.f;
        player.getBody()->SetLinearVelocity(vel);
        m_time = 0.f;
        --m_leftTimes;
        return true;
    }
    return false;
}

bool PlayerDashSkill::tick(Player& player, float deltaTime)
{
    m_time += deltaTime;
    if (m_time > 0.3f)
    {
		player.getAnimator()->popState();
        player.getBody()->SetGravityScale(1.f);
        player.stopHorizontalMovement();
        return true;
    }
    return false;
}
