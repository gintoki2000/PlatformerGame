#include "Kobold.h"
#include "Animation.h"
#include "Animator.h"
#include "Constances.h"
#include "Enums.h"
#include "GameObject.h"
#include "Level.h"
#include "Monster.h"
#include "Player.h"
#include "SpriteSheet.h"

Koblod::Koblod(Level* level) : Monster(level, MONSTER_TYPE_KOBLOD, 1)
{
    m_width  = WIDTH;
    m_height = HEIGHT;
    SDL_Texture* texture =
        level->getTextureManager()->get("asserts/spritesheets/kobold.png");
    m_spriteSheet = new SpriteSheet(texture, SPRITE_WIDTH, SPRITE_HEIGHT);

    Animation* anims[NUM_ANIMS];

    anims[ANIM_IDLE]   = new Animation(m_spriteSheet, 0, 8, 1.f / 8.f);
    anims[ANIM_RUN]    = new Animation(m_spriteSheet, 8, 12, 1.f / 10.f);
    anims[ANIM_ATTACK] = new Animation(m_spriteSheet, 20, 4, 1.f / 10.f);
    anims[ANIM_HURT]   = new Animation(m_spriteSheet, 24, 3, 1.f / 8.f);
    anims[ANIM_DIE]    = new Animation(m_spriteSheet, 27, 8, 1.f / 8.f);

    anims[ANIM_IDLE]->setPlayMode(Animation::PLAY_MODE_LOOP);
	anims[ANIM_RUN]->setPlayMode(Animation::PLAY_MODE_LOOP);

    m_animator = new Animator(anims, NUM_ANIMS);
	m_animator->setOriginX(SPRITE_WIDTH / 2);
	m_animator->setOriginY(SPRITE_WIDTH / 2);

    b2BodyDef bdef;
    bdef.fixedRotation = true;
    bdef.userData      = this;
    bdef.type          = b2_dynamicBody;

    m_body = m_level->getWorld()->CreateBody(&bdef);

    b2PolygonShape box;
    box.SetAsBox(WIDTH / 2.f / Constances::PPM, HEIGHT / 2.f / Constances::PPM);

    b2FixtureDef fdef;
    fdef.filter.categoryBits = CATEGORY_BIT_MONSTER;
    fdef.filter.maskBits     = CATEGORY_BIT_BLOCK | CATEGORY_BIT_PLAYER;
    fdef.shape               = &box;
    m_body->CreateFixture(&fdef);
    resetMembers();
}

Koblod::~Koblod()
{
    delete m_spriteSheet;
    m_spriteSheet = nullptr;
}

void Koblod::resetMembers()
{
	m_direction = DIRECTION_RIGHT;
    idle();
    setPosition(0.f, 0.f);
}

class KoblodAttackCallback : public b2QueryCallback 
{
	bool ReportFixture(b2Fixture *fixture) override
	{
		auto userData = fixture->GetBody()->GetUserData();
		if (userData != nullptr && ((GameObject*) userData)->getGameObjectType() == GAME_OBJECT_TYPE_PLAYER)
		{
			auto player = (Player*)userData;
			player->getHit(2);
		}
		return true;
	}
};
void Koblod::updateLogic(float deltaTime)
{
    m_timer += deltaTime;
	m_direction = (Direction)getFacingPlayerDirection();
    switch (m_state)
    {
    case STATE_IDLE:
    {
		if (getDistanceToPlayer() <= ACTIVATE_RUN_DIS)
		{
			run();
		}
    }
    break;
    case STATE_RUN:
    {
		auto distanceToPlayer = getDistanceToPlayer();
		if (distanceToPlayer > ACTIVATE_RUN_DIS)
		{
			idle();
		}
		else if(distanceToPlayer <= ACTIVATE_ATK_DIS)  
		{
			attack();
		}
		else 
		{
			int sign = m_direction == DIRECTION_LEFT ? -1 : 1;
			setHorizontalSpeed(sign * 3.f);
		}
    }
    break;
    case STATE_ATTACK:
    {
		if (m_animator->isCurrentAnimationFinshed())
		{
			b2AABB area;
			KoblodAttackCallback callback;
			if (m_direction == DIRECTION_LEFT)
			{
				area.lowerBound.x = m_body->GetPosition().x - 2.f;
				area.lowerBound.y = m_body->GetPosition().y - 4.f / Constances::PPM;
				area.upperBound.x = m_body->GetPosition().x;
				area.upperBound.y = m_body->GetPosition().y + 4.f / Constances::PPM;
			}
			else 
			{
				area.lowerBound.x = m_body->GetPosition().x;
				area.lowerBound.y = m_body->GetPosition().y - 4.f / Constances::PPM;
				area.upperBound.x = m_body->GetPosition().x + 2.f;
			   	area.upperBound.y = m_body->GetPosition().y + 4.f / Constances::PPM;	
			}
			m_level->getWorld()->QueryAABB(&callback, area);
			wait();
		}
    }
    break;
    case STATE_WAIT:
    {
		if (m_animator->getElapsedTime() > 2.f)
		{
			m_state = STATE_IDLE;
		}
    }
    break;
    case STATE_HURT:
    {
		if (m_animator->isCurrentAnimationFinshed())
		{
			wait();
		}
    }
    break;
    case STATE_DIE:
    {
		if (m_animator->isCurrentAnimationFinshed())
		{
			m_level->removeMonster(this);
		}
    }
    break;
    }
}

void Koblod::idle()
{
    m_state = STATE_IDLE;
    m_animator->play(ANIM_IDLE, 0.f);
	stopHorizontalMovement();
}

void Koblod::run()
{
	m_state = STATE_RUN;
	m_animator->play(ANIM_RUN, 0.f);
}

void Koblod::attack()
{
	m_state = STATE_ATTACK;
	m_animator->play(ANIM_ATTACK, 0.f);
	stopHorizontalMovement();
}

void Koblod::wait()
{
	m_state = STATE_WAIT;
	m_animator->play(ANIM_IDLE, 0.f);
}

void Koblod::hurt()
{
	m_state = STATE_HURT;
	m_animator->play(ANIM_HURT, 0.f);
	stopHorizontalMovement();
}

void Koblod::die()
{
	m_state = STATE_DIE;
	m_animator->play(ANIM_DIE, 0.f);
	stopHorizontalMovement();
}

void Koblod::getHit(int damage)
{
	if (m_state != STATE_DIE && m_state != STATE_HURT)
	{
		Monster::getHit(damage);
		if (m_hitPoints == 0)
		{
			die();
		}
		else 
		{
			hurt();
		}
	}	
}
