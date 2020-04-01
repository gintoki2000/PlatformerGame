#include "Slime.h"
#include "Animation.h"
#include "Animator.h"
#include "Constances.h"
#include "Enums.h"
#include "Level.h"
#include "Locator.h"
#include "Monster.h"
#include "NTRect.h"
#include "Player.h"
#include "SDL_image.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SpriteSheet.h"
#include <memory>
class Player;

Slime::Slime(Level* level) :
    Monster(level, MONSTER_TYPE_SLIME, 10)
{
	Animation* anims[NUM_ANIMS];
    auto texture   = level->getTextureManager()->get("asserts/spritesheets/slime.png");

	m_spriteSheet = new SpriteSheet(texture, SPRITE_WIDTH, SPRITE_HEIGHT);
    anims[ANIM_IDLE]   = new Animation(m_spriteSheet, 0, 4, 1.f / 4.f);
    anims[ANIM_MOVE]   = new Animation(m_spriteSheet, 4, 4, 1.f / 4.f);
    anims[ANIM_ATTACK] = new Animation(m_spriteSheet, 8, 5, 1.f / 6.f);
    anims[ANIM_HURT]   = new Animation(m_spriteSheet, 13, 4, 1.f / 8.f);
    anims[ANIM_DIE]    = new Animation(m_spriteSheet, 17, 4, 1.f / 8.f);

    anims[ANIM_IDLE]->setPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIM_MOVE]->setPlayMode(Animation::PLAY_MODE_LOOP);

    m_animator = new Animator(anims, NUM_ANIMS);
    m_animator->setOriginX(SPRITE_WIDTH / 2);
    m_animator->setOriginY(SPRITE_HEIGHT / 2);

    b2BodyDef bdef;
    bdef.fixedRotation = true;
    bdef.type          = b2_dynamicBody;
    bdef.userData      = this;

    m_body = level->getWorld()->CreateBody(&bdef);

    b2PolygonShape box;
    float          widthInMeter  = WIDTH / Constances::PPM;
    float          heightInMeter = HEIGHT / Constances::PPM;
    box.SetAsBox(widthInMeter / 2.f, heightInMeter / 2.f);

    b2FixtureDef fdef;
    fdef.shape               = &box;
    fdef.filter.categoryBits = CATEGORY_BIT_MONSTER;
    fdef.filter.maskBits     = CATEGORY_BIT_BLOCK | CATEGORY_BIT_PLAYER;

    m_body->CreateFixture(&fdef);
    resetMembers();
}

Slime::~Slime()
{
	delete m_spriteSheet;
	m_spriteSheet = nullptr;
}


void Slime::resetMembers()
{
    m_animator->play(ANIM_IDLE, 0.f);
    setPosition(0, 0);
    m_direction = DIRECTION_LEFT;
    idle();
}
void Slime::updateLogic(float deltaTime)
{
    m_timer += deltaTime;
    m_changingDirTimer += deltaTime;
    checkDirection();
    switch (m_state)
    {
    case STATE_IDLE:
    {
        auto distanceToPlayer = getDistanceToPlayer();
        if (distanceToPlayer < ACTIVATE_DISTANCE)
        {
            move();
        }
        else if (distanceToPlayer <= ATTACK_DISTANCE)
        {
            attack();
        }
    }
    break;
    case STATE_WAIT:
    {
        if (m_timer > 1.f)
        {
            idle();
        }
    }
    break;
    case STATE_MOVE:
    {
        auto distanceToPlayer = getDistanceToPlayer();
        if (distanceToPlayer > ACTIVATE_DISTANCE)
        {
            idle();
        }
        else if (distanceToPlayer <= ATTACK_DISTANCE)
        {
            attack();
        }
        else
        {
            int sign = m_direction == DIRECTION_LEFT ? -1 : 1;
            setHorizontalSpeed(Slime::MOVE_SPEED * sign);
        }
    }
    break;
    case STATE_ATTACK:
    {
        if (m_animator->isCurrentAnimationFinshed())
        {
            checkAttackCollision();
            wait();
        }
        else
        {
        }
    }
    break;
    case STATE_HURT:
    {
    }
    break;
    case STATE_DIE:
    {
    }
    break;
    }
}


void Slime::checkDirection()
{
    if (m_changingDirTimer > 1.5f)
    {
        m_changingDirTimer = 0.f;
        m_direction        = getFacingPlayerDirection();
    }
}


void Slime::getHit(int damage)
{
    if (m_state != STATE_HURT && m_state != STATE_DIE)
    {
        Monster::getHit(damage);
        if (m_hitPoints == 0)
        {
            m_state = STATE_DIE;
            m_animator->play(ANIM_DIE, 0.f);
        }
        else
        {
            m_state = STATE_HURT;
            m_animator->play(ANIM_HURT, 0.f);
        }
    }
}
class SlimeAttackingCallBack : public b2QueryCallback
{
  public:
    SlimeAttackingCallBack(Player* player) : m_player(player) {}

    bool ReportFixture(b2Fixture* fixture) override
    {
        void* obj = fixture->GetBody()->GetUserData();
        if (obj == m_player)
        {
            m_player->getHit(1);
            return false;
        }
        return true;
    }

  private:
    Player* m_player;
};
void Slime::checkAttackCollision()
{
    b2AABB area;
    area.lowerBound.x = (m_positionX - WIDTH / 2.f) / Constances::PPM;
    area.lowerBound.y = (m_positionY - HEIGHT / 2.f) / Constances::PPM;
    area.upperBound.x = (m_positionX + WIDTH / 2.f) / Constances::PPM;
    area.upperBound.y = (m_positionY + HEIGHT / 2.f) / Constances::PPM;

    SlimeAttackingCallBack callback(m_level->getPlayer());
    m_body->GetWorld()->QueryAABB(&callback, area);
}
void Slime::setState(State newState, float initialTime)
{
    m_state = newState;
    m_timer = initialTime;
}
void Slime::idle()
{
    setState(STATE_IDLE, 0.f);
    m_animator->play(ANIM_IDLE, 0.f);
    stopHorizontalMovement();
}

void Slime::wait()
{
    setState(STATE_WAIT, 0.f);
    m_animator->play(ANIM_IDLE, 0.f);
    stopHorizontalMovement();
}

void Slime::move()
{
    setState(STATE_MOVE, 0.f);
    m_animator->play(ANIM_MOVE, 0.f);
}

void Slime::attack()
{
    setState(STATE_ATTACK, 0.f);
    m_animator->play(ANIM_ATTACK, 0.f);
}

void Slime::hurt()
{
    setState(STATE_HURT, 0.f);
    m_animator->play(ANIM_HURT, 0.f);
}

void Slime::die()
{
    setState(STATE_DIE, 0.f);
    m_animator->play(ANIM_DIE, 0.f);
}

