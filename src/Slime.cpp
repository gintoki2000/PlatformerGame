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
Slime::Asserts Slime::sharedAsserts = Slime::Asserts();

Slime::Asserts::Asserts()
{
    texture     = nullptr;
    spriteSheet = nullptr;
    count       = 0;
}

Slime::Asserts::~Asserts()
{
    delete spriteSheet;
    SDL_DestroyTexture(texture);
    spriteSheet = nullptr;
    texture     = nullptr;
}

bool Slime::Asserts::load()
{
    if (count == 0)
    {
        if ((texture = IMG_LoadTexture(Locator::getRenderer(),
                                       "asserts/slime.png")) != nullptr)
        {
            spriteSheet = new SpriteSheet(texture, SPRITE_WIDTH, SPRITE_HEIGHT);
            return true;
        }
        return false;
    }
    return true;
}
void Slime::Asserts::incCount() { ++count; }

void Slime::Asserts::decCount()
{
    --count;
    if (count == 0)
    {
        this->~Asserts();
    }
}

Slime* Slime::create(Level* level)
{
    Slime* ret = new Slime;
    if (ret->init(level))
    {
        return ret;
    }
    delete ret;
    return nullptr;
}
Slime* Slime::create(Level* level, float cx, float cy)
{
    Slime* ret;
    if ((ret = create(level)) != nullptr)
    {
        ret->setPosition(cx, cy);
    }
    return ret;
}
Slime::~Slime()
{
    delete m_animator;
    m_body->GetWorld()->DestroyBody(m_body);
    m_animator = nullptr;
    m_body     = nullptr;
    sharedAsserts.decCount();
}

bool Slime::init(Level* level)
{

    Monster::init(MONSTER_TYPE_SLIME, level, 10);
    Animation* anims[NUM_ANIMS];

    if (!sharedAsserts.load())
    {
        SDL_Log("Failed to load asserts!");
        return false;
    }
    sharedAsserts.incCount();
    auto spriteSheet   = sharedAsserts.spriteSheet;
    anims[ANIM_IDLE]   = new Animation(spriteSheet, 0, 4, 1.f / 4.f);
    anims[ANIM_MOVE]   = new Animation(spriteSheet, 4, 4, 1.f / 4.f);
    anims[ANIM_ATTACK] = new Animation(spriteSheet, 8, 5, 1.f / 6.f);
    anims[ANIM_HURT]   = new Animation(spriteSheet, 13, 4, 1.f / 8.f);
    anims[ANIM_DIE]    = new Animation(spriteSheet, 17, 4, 1.f / 8.f);

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
    return true;
}

void Slime::resetMembers()
{
    m_animator->play(ANIM_IDLE, 0.f);
    setPosition(0, 0);
    m_direction = DIRECTION_LEFT;
    idle();
}
void Slime::updateGraphics(float deltaTime)
{
    const NTRect& viewport = m_level->getViewport();
    NTRect        boundingBox;
    boundingBox.x = m_positionX - WIDTH / 2.f;
    boundingBox.y = m_positionY - HEIGHT / 2.f;
    boundingBox.w = WIDTH;
    boundingBox.h = HEIGHT;
    if (SDL_HasIntersection(&viewport, &boundingBox))
    {
        setVisible(true);
        m_animator->tick(deltaTime);
        m_animator->setPositionX(m_positionX - viewport.x);
        m_animator->setPositionY(m_positionY - viewport.y);
        m_animator->setFlip(m_direction == DIRECTION_LEFT
                                ? SDL_FLIP_NONE
                                : SDL_FLIP_HORIZONTAL);
        m_animator->setRotation(m_rotation);
    }
    else
    {
        setVisible(false);
    }
}

void Slime::updatePhysics()
{
    const auto& position = m_body->GetPosition();
    m_positionX          = position.x * Constances::PPM;
    m_positionY          = position.y * Constances::PPM;
    m_rotation           = (double)m_body->GetAngle();
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
			attackPlayer();
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

void Slime::onPositionChanged() { synchronizeBodyTransform(); }

void Slime::synchronizeBodyTransform()
{
    b2Vec2 pos(m_positionX / Constances::PPM, m_positionY / Constances::PPM);
    m_body->SetTransform(pos, (float)m_rotation);
}

void Slime::setHorizontalSpeed(float speed)
{
    b2Vec2 vel = m_body->GetLinearVelocity();
    vel.x      = speed;
    m_body->SetLinearVelocity(vel);
}

void Slime::checkDirection()
{
    if (m_changingDirTimer > 1.5f)
    {
        m_changingDirTimer = 0.f;
        m_direction        = getFacingPlayerDirection();
    }
}

void Slime::tick(float deltaTime)
{
    updatePhysics();
    updateGraphics(deltaTime);
    updateLogic(deltaTime);
}

void Slime::paint()
{
    if (isVisible())
    {
        m_animator->paint(Locator::getRenderer());
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
void Slime::attackPlayer()
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

void Slime::stopHorizontalMovement() { setHorizontalSpeed(0.f); }
