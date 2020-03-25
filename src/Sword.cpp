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
    m_phrase      = 0;
    m_isActive    = true;
    m_chainAttack = false;
    m_lastAnim    = m_player->m_animator->getAnimationIndex();
    m_player->m_animator->play(Player::ANIM_ATK_1, 0.f);
}

void Sword::cancel() { m_isActive = false; }

bool Sword::tick(float)
{
    if (m_isActive)
    {
        switch (m_phrase)
        {
        case 0:
        {
            if (Input::isButtonAJustPressed())
            {
                m_chainAttack = true;
            }
            if (m_player->m_animator->isCurrentAnimationFinshed())
            {
                if (m_chainAttack)
                {
                    m_phrase      = 1;
                    m_chainAttack = false;
                    m_player->m_animator->play(Player::ANIM_ATK_2, 0.f);
                }
                else
                {
                    m_isActive = false;
                    m_player->m_animator->play(m_lastAnim, 0.f);
                }
            }
        }
        break;
        case 1:
        {
            if (Input::isButtonAJustPressed())
            {
                m_chainAttack = true;
            }
            if (m_player->m_animator->isCurrentAnimationFinshed())
            {
                if (m_chainAttack)
                {
                    m_phrase      = 2;
                    m_chainAttack = false;
                    m_player->m_animator->play(Player::ANIM_ATK_3, 0.f);
                }
                else
                {
                    m_isActive = false;
                    m_player->m_animator->play(m_lastAnim, 0.f);
                }
            }
        }
        break;
        case 2:
        {
            if (m_player->m_animator->isCurrentAnimationFinshed())
            {
                m_isActive = false;
                m_player->m_animator->play(m_lastAnim, 0.f);
            }
        }
        }
    }
    return m_isActive;
}
