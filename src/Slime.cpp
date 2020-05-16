#include "Slime.h"
/*#include "Animation.h"
#include "Animator.h"
#include "Constances.h"
#include "Enums.h"
#include "Level.h"
#include "Locator.h"
#include "Monster.h"
#include "NTRect.h"
#include "Adventurer.h"
#include "SDL_image.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SpriteSheet.h"
#include <memory>
class Adventurer;

Slime::Slime(Level* level) :
    Monster(level, MONSTER_TYPE_SLIME, 10)
{
	Animation* anims[NUM_ANIMS];
    auto texture   = level->GetTextureManager()->Get("asserts/spritesheets/slime.png");

	m_spriteSheet = new SpriteSheet(texture, SPRITE_WIDTH, SPRITE_HEIGHT);
    anims[ANIm_IDLE]   = new Animation(m_spriteSheet, 0, 4, 1.f / 4.f);
    anims[ANIm_MOVE]   = new Animation(m_spriteSheet, 4, 4, 1.f / 4.f);
    anims[ANIm_ATTACK] = new Animation(m_spriteSheet, 8, 5, 1.f / 6.f);
    anims[ANIm_HURT]   = new Animation(m_spriteSheet, 13, 4, 1.f / 8.f);
    anims[ANIm_DIE]    = new Animation(m_spriteSheet, 17, 4, 1.f / 8.f);

    anims[ANIm_IDLE]->SetPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIm_MOVE]->SetPlayMode(Animation::PLAY_MODE_LOOP);

    m_animator = new Animator(anims, NUM_ANIMS);
    m_animator->SetOriginX(SPRITE_WIDTH / 2);
    m_animator->SetOriginY(SPRITE_HEIGHT / 2);

    b2BodyDef bdef;
    bdef.fixedRotation = true;
    bdef.type          = b2_dynamicBody;
    bdef.userData      = this;

    m_body = level->GetWorld()->CreateBody(&bdef);

    b2PolygonShape box;
    float          widthInMeter  = WIDTH / Constances::PPM;
    float          heightInMeter = HEIGHT / Constances::PPM;
    box.SetAsBox(widthInMeter / 2.f, heightInMeter / 2.f);

    b2FixtureDef fdef;
    fdef.shape               = &box;
    fdef.filter.categoryBits = CATEGORY_BIT_MONSTER;
    fdef.filter.maskBits     = CATEGORY_BIT_BLOCK | CATEGORY_BIT_PLAYER;

    m_body->CreateFixture(&fdef);
    ResetMembers();
}

Slime::~Slime()
{
	delete m_spriteSheet;
	m_spriteSheet = nullptr;
}


void Slime::ResetMembers()
{
    m_animator->Play(ANIm_IDLE, 0.f);
    SetPosition(0, 0);
    m_direction = DIRECTION_LEFT;
    Idle();
}
void Slime::UpdateLogic(float deltaTime)
{
    m_timer += deltaTime;
    m_changingDirTimer += deltaTime;
    CheckDirection();
    switch (m_state)
    {
    case STATE_IDLE:
    {
        auto distanceToAdventurer = GetDistanceToAdventurer();
        if (distanceToAdventurer < ACTIVATE_DISTANCE)
        {
            Move();
        }
        else if (distanceToAdventurer <= ATTACK_DISTANCE)
        {
            Attack();
        }
    }
    break;
    case STATE_WAIT:
    {
        if (m_timer > 1.f)
        {
            Idle();
        }
    }
    break;
    case STATE_MOVE:
    {
        auto distanceToAdventurer = GetDistanceToAdventurer();
        if (distanceToAdventurer > ACTIVATE_DISTANCE)
        {
            Idle();
        }
        else if (distanceToAdventurer <= ATTACK_DISTANCE)
        {
            Attack();
        }
        else
        {
            int sign = m_direction == DIRECTION_LEFT ? -1 : 1;
            SetHorizontalSpeed(Slime::MOVE_SPEED * sign);
        }
    }
    break;
    case STATE_ATTACK:
    {
        if (m_animator->IsCurrentAnimationFinshed())
        {
            CheckAttackCollision();
            Wait();
        }
    }
    break;
    case STATE_HURT:
    {
		if (m_animator->IsCurrentAnimationFinshed())
		{
			Wait();
		}
    }
    break;
    case STATE_DIE:
    {
		if (m_animator->IsCurrentAnimationFinshed())
		{
			m_level->RemoveMonster(this);
		}
    }
    break;
    }
}


void Slime::CheckDirection()
{
    if (m_changingDirTimer > 1.5f)
    {
        m_changingDirTimer = 0.f;
        m_direction        = GetFacingAdventurerDirection();
    }
}


void Slime::GetHit(int damage)
{
    if (m_state != STATE_HURT && m_state != STATE_DIE)
    {
        Monster::GetHit(damage);
        if (m_hitPoints == 0)
        {
            m_state = STATE_DIE;
            m_animator->Play(ANIm_DIE, 0.f);
        }
        else
        {
            m_state = STATE_HURT;
            m_animator->Play(ANIm_HURT, 0.f);
        }
    }
}
class SlimeAttackingCallBack : public b2QueryCallback
{
  public:
    SlimeAttackingCallBack(Adventurer* adventurer) : m_adventurer(adventurer) {}

    bool ReportFixture(b2Fixture* fixture) override
    {
        void* obj = fixture->GetBody()->GetUserData();
        if (obj == m_adventurer)
        {
            m_adventurer->GetHit(1);
            return false;
        }
        return true;
    }

  private:
    Adventurer* m_adventurer;
};
void Slime::CheckAttackCollision()
{
    b2AABB area;
    area.lowerBound.x = (m_positionX - WIDTH / 2.f) / Constances::PPM;
    area.lowerBound.y = (m_positionY - HEIGHT / 2.f) / Constances::PPM;
    area.upperBound.x = (m_positionX + WIDTH / 2.f) / Constances::PPM;
    area.upperBound.y = (m_positionY + HEIGHT / 2.f) / Constances::PPM;

    SlimeAttackingCallBack Callback(m_level->GetAdventurer());
    m_body->GetWorld()->QueryAABB(&callback, area);
}
void Slime::SetState(State newState, float initialTime)
{
    m_state = newState;
    m_timer = initialTime;
}
void Slime::Idle()
{
    SetState(STATE_IDLE, 0.f);
    m_animator->Play(ANIm_IDLE, 0.f);
    StopHorizontalMovement();
}

void Slime::Wait()
{
    SetState(STATE_WAIT, 0.f);
    m_animator->Play(ANIm_IDLE, 0.f);
    StopHorizontalMovement();
}

void Slime::Move()
{
    SetState(STATE_MOVE, 0.f);
    m_animator->Play(ANIm_MOVE, 0.f);
}

void Slime::Attack()
{
    SetState(STATE_ATTACK, 0.f);
    m_animator->Play(ANIm_ATTACK, 0.f);
}

void Slime::Hurt()
{
    SetState(STATE_HURT, 0.f);
    m_animator->Play(ANIm_HURT, 0.f);
}

void Slime::Die()
{
    SetState(STATE_DIE, 0.f);
    m_animator->Play(ANIm_DIE, 0.f);
}
*/
