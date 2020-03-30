#include "Sword.h"
#include "Animator.h"
#include "Input.h"
#include "Player.h"
Sword::Sword() {}

Sword::~Sword() {}

void Sword::start()
{
    if (isActive())
    {
        return;
    }
    m_phrase   = 0;
    m_isActive = true;
    m_chain    = false;
    m_lastAnim = m_player->getAnimator()->getAnimationIndex();
    m_player->getAnimator()->play(Player::ANIM_ATK_1, 0.f);
}

void Sword::cancel() { m_isActive = false; }

bool Sword::tick(float)
{
    if (isActive())
    {
        switch (m_phrase)
        {
        case 0:
		{
			if (m_player->getAnimator()->isCurrentAnimationFinshed())
			{
				m_isActive = false;
				m_player->getAnimator()->play(m_lastAnim, 0.f);
			}
		}
		break;
        }
    }
    return m_isActive;
}
