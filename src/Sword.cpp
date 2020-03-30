#include "Sword.h"
#include "Animator.h"
#include "Constances.h"
#include "Enums.h"
#include "GameObject.h"
#include "Input.h"
#include "Monster.h"
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
            if (Input::isButtonAJustPressed())
            {
                m_chain = true;
            }
            if (m_player->getAnimator()->isCurrentAnimationFinshed())
            {
				query();
                if (m_chain)
                {
                    m_phrase = 1;
                    m_player->getAnimator()->play(Player::ANIM_ATK_2, 0.f);
                }
                else
                {
                    m_isActive = false;
                    m_player->setState(new PlayerIdle2State());
                }
            }
        }
        break;
        case 1:
        {
            if (Input::isButtonAJustPressed())
            {
                m_chain = true;
            }
            if (m_player->getAnimator()->isCurrentAnimationFinshed())
            {
				query();
                if (m_chain)
                {
                    m_phrase = 2;
                    m_player->getAnimator()->play(Player::ANIM_ATK_3, 0.f);
                }
                else
                {
                    m_isActive = false;
                    m_player->setState(new PlayerIdle2State());
                }
            }
        }
        case 2:
        {
            if (m_player->getAnimator()->isCurrentAnimationFinshed())
            {
				query();
                m_isActive = false;
                m_player->setState(new PlayerIdle2State());
            }
        }
        break;
        }
    }
    return m_isActive;
}

class SwordAttackCallback : public b2QueryCallback
{
	bool ReportFixture(b2Fixture *fixture) override
	{
		void* userData = fixture->GetBody()->GetUserData();
		if (userData != nullptr)
		{
			if (((GameObject*) userData)->getGameObjectType() == GAME_OBJECT_TYPE_MONSTER)
			{
				Monster* monster = (Monster*)userData;
				monster->getHit(1);
				SDL_Log("attack->%p", (void*)monster);
			}
		}
		return true;
	}
};

void Sword::query()
{
	b2AABB area;
	area.lowerBound.x = (m_player->getPositionX() - Player::WIDTH) / Constances::PPM;	
	area.lowerBound.y = (m_player->getPositionY() - Player::HEIGHT) / Constances::PPM;	
	area.upperBound.x = (m_player->getPositionX() + Player::WIDTH) / Constances::PPM;	
	area.upperBound.y = (m_player->getPositionY() + Player::HEIGHT) / Constances::PPM;	

	SwordAttackCallback callback;
	m_player->getBody()->GetWorld()->QueryAABB(&callback, area);
}
