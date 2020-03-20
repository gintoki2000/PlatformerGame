#include "Slime.h"
#include "Animation.h"
#include "Animator.h"
#include "AssertManager.h"
#include "Constances.h"
#include "Enums.h"
#include "Level.h"
#include "Locator.h"
#include "Monster.h"
#include "NTRect.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SpriteSheet.h"
#include <memory>
std::shared_ptr<SpriteSheet> Slime::sharedSpriteSheet = nullptr;
class Player;
Slime::Slime(Level* level) :
    Monster(MONSTER_TYPE_SLIME, 10),
    m_body(nullptr),
    m_spriteSheet(nullptr),
    m_animator(nullptr)
{
    Animation* animations[NUM_ANIMS];

    if (sharedSpriteSheet == nullptr)
    {
        AssertManager* asserts = level->getAssertManager();
        SDL_Texture*   texture =
            asserts->getAssert<SDL_Texture>("asserts/slime.png");
        sharedSpriteSheet =
            std::make_shared<SpriteSheet>(texture, SPRITE_WIDTH, SPRITE_HEIGHT);
    }

    m_spriteSheet = sharedSpriteSheet;
    animations[ANIM_IDLE] = new Animation(m_spriteSheet.get(), 0, 4, 1.f / 8.f,
                                          Animation::PLAY_MODE_LOOP);
    animations[ANIM_MOVE] = new Animation(m_spriteSheet.get(), 5, 4, 1.f / 8.f,
                                          Animation::PLAY_MODE_LOOP);
    animations[ANIM_ATTACK] = new Animation(
        m_spriteSheet.get(), 9, 5, 1.f / 8.f, Animation::PLAY_MODE_NORMAL);
    animations[ANIM_HURT] = new Animation(m_spriteSheet.get(), 14, 4, 1.f / 8.f,
                                          Animation::PLAY_MODE_NORMAL);
    animations[ANIM_DIE] = new Animation(m_spriteSheet.get(), 19, 4, 1.f / 8.f,
                                         Animation::PLAY_MODE_NORMAL);
    m_animator = new Animator(animations, NUM_ANIMS);

	b2BodyDef bdef;
	bdef.fixedRotation = true;
	bdef.type = b2_dynamicBody;
	bdef.userData = this;
	
	m_body = level->getWorld()->CreateBody(&bdef);

	b2PolygonShape box;
	float widthInMeter = WIDTH / Constances::PPM;
	float heightInMeter = HEIGHT / Constances::PPM;
	box.SetAsBox(widthInMeter / 2.f, heightInMeter / 2.f);

	b2FixtureDef fdef;
	fdef.shape = &box;
	fdef.filter.categoryBits = CATEGORY_BIT_MONSTER;
	fdef.filter.maskBits = CATEGORY_BIT_BLOCK | CATEGORY_BIT_PLAYER;

	m_body->CreateFixture(&fdef);
}

Slime::~Slime()
{
    delete m_animator;
    m_animator = nullptr;
    m_spriteSheet = nullptr;
    m_body->GetWorld()->DestroyBody(m_body);
    m_body = nullptr;
}

void Slime::resetMembers()
{
    m_animator->play(ANIM_IDLE, 0.f);
    setPosition(0, 0);
    m_direction = DIRECTION_LEFT;
    m_state = STATE_IDLE;
    m_timer = 0.f;
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
    m_positionX = position.x * Constances::PPM;
    m_positionY = position.y * Constances::PPM;
    m_rotation = (double)m_body->GetAngle();
}

void Slime::updateLogic(float deltaTime)
{
    m_timer += deltaTime;
    switch (m_state)
    {
    case STATE_IDLE:
    {
        if (getDistanceToPlayer() <= ACTIVATE_DISTANCE)
        {
            m_state = STATE_MOVE;
            m_timer = 0.f;
            m_animator->play(ANIM_MOVE, 0.f);
        }
    }
    break;
    case STATE_WAIT:
    {
        if (m_timer > 1.f)
        {
            m_state = STATE_IDLE;
            m_timer = 0.f;
        }
    }
    break;
    case STATE_MOVE:
    {
        if (getDistanceToPlayer() <= ATTACK_DISTANCE)
        {
            m_state = STATE_ATTACK;
            m_timer = 0.f;
            m_animator->play(ANIM_ATTACK, 0.f);
        }
        else
        {
            int sign = m_direction == DIRECTION_LEFT ? -1 : 1;
            setHorizontalSpeed(MOVE_SPEED * sign);
        }
    }
    break;
    case STATE_ATTACK:
    {
        if (m_animator->isCurrentAnimFinshed())
		{
			attackPlayer();
			m_state = STATE_WAIT;
			m_timer = 0.f;
			m_animator->play(ANIM_IDLE, 0.f);
		}
    }
    break;
    case STATE_HURT:
    {
        if (m_animator->isCurrentAnimFinshed())
        {
            m_state = STATE_IDLE;
            m_timer = 0.f;
            m_animator->play(ANIM_IDLE, 0.f);
        }
    }
    break;
    case STATE_DIE:
    {
        if (m_animator->isCurrentAnimFinshed())
        {
            m_level->removeMonster(this);
        }
    }
    break;
    }
}

void Slime::onPositionChanged()
{
    // m_physics->synchronizeTransformWithOwner()
    synchronizeBodyTransform();
}

void Slime::synchronizeBodyTransform()
{
    b2Vec2 pos(m_positionX / Constances::PPM, m_positionY / Constances::PPM);
    m_body->SetTransform(pos, (float)m_rotation);
}

void Slime::setHorizontalSpeed(float speed)
{
    b2Vec2 vel = m_body->GetLinearVelocity();
    vel.x = speed;
    m_body->SetLinearVelocity(vel);
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
		m_animator->render(Locator::getRenderer());
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
            // TODO call player's getHit method
			// m_player->getHit(2);
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
