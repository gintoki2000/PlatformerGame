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
    int    mouseX, mouseY;
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
    if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
        m_phrase = 0;
        m_timer  = 0.f;

        player.getAnimator()->pushState(AnimatorState(Player::ANIM_CAST_SPELL));
        player.stopHorizontalMovement();

        const SDL_Rect& viewport = player.getLevel()->getViewport();

        mouseX = mouseX / Constances::SCALE_X + viewport.x;
        mouseY = mouseY / Constances::SCALE_Y + viewport.y;

        Level* level   = player.getLevel();
        b2Vec2 initPos = player.getBody()->GetPosition();
        b2Vec2 speed =
            b2Vec2(mouseX / Constances::PPM, mouseY / Constances::PPM) -
            initPos;
		float magnitude = speed.Length();
		speed.x /= magnitude;
		speed.y /= magnitude;
		speed.x *= 16.f;
		speed.y *= 16.f;
        Fireball* fireball = new Fireball(level, initPos, speed);
        level->addFireball(fireball);

        return true;
    }
    return false;
}

bool PlayerFireballSkill::tick(Player& player, float)
{
    if (player.getAnimator()->isCurrentAnimationFinshed())
    {
        player.getAnimator()->popState();
        return true;
    }
    return false;
}
