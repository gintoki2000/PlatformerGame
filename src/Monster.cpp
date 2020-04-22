#include "Monster.h"
#include "Constances.h"
#include "Player.h"
#include "Utils.h"
#include "WorldManager.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_world.h"

Monster::Monster()
{
    m_body              = nullptr;
    m_hitPoints         = 0;
    m_maxHitPoints      = 0;
    m_identifier.tag    = TAG_MONSTER;
    m_identifier.object = this;
}

Monster::~Monster()
{
    m_body->GetWorld()->DestroyBody(m_body);
    m_body = nullptr;
}

bool Monster::init(const FloatRect& aabb)
{
    b2World&  world = *WorldManager::getWorld();
    b2BodyDef bdef;
    bdef.fixedRotation = true;
    bdef.type          = b2_dynamicBody;
    bdef.userData      = &m_identifier;
    bdef.position.x    = aabb.x + aabb.width / 2.f;
    bdef.position.y    = aabb.y + aabb.height / 2.f;
    bdef.position.x /= Constances::PPM;
    bdef.position.y /= Constances::PPM;

    m_body = world.CreateBody(&bdef);

    b2PolygonShape box;

    float hw = aabb.width / 2.f;
    float hh = aabb.height / 2.f;
    hw /= Constances::PPM;
    hh /= Constances::PPM;
    float bhh = hh - hw;
    float cx  = 0.f;
    float cy  = -(hh - bhh);
    box.SetAsBox(hw, bhh, b2Vec2(cx, cy), 0.f);

    b2FixtureDef fdef;
    fdef.filter.categoryBits = CATEGORY_BIT_MONSTER;
    fdef.filter.maskBits =
        CATEGORY_BIT_PLAYER | CATEGORY_BIT_BLOCK | CATEGORY_BIT_SPELL;
    fdef.restitution = 0;
    fdef.shape       = &box;

    m_body->CreateFixture(&fdef);

    b2CircleShape circle;
    circle.m_radius = hw;
    circle.m_p.x    = 0.f;
    circle.m_p.y    = hh - hw;

    fdef.shape    = &circle;
    fdef.friction = 0.5f;

    m_body->CreateFixture(&fdef);

    m_identifier.tag    = TAG_MONSTER;
    m_identifier.object = this;

    m_damageWhenTouching = 1.f;
    return true;
}

void Monster::tick(float)
{
    m_positionX = m_body->GetPosition().x * Constances::PPM;
    m_positionY = m_body->GetPosition().y * Constances::PPM;
}

void Monster::takeDamge(int damage)
{
    if (!isDead())
    {
        m_hitPoints -= damage;
        if (m_hitPoints < 0)
        {
            m_hitPoints = 0;
        }
    }
}

int  Monster::getHitPoints() { return m_hitPoints; }
int  Monster::getMaxHitPoints() { return m_maxHitPoints; }
bool Monster::isDead() { return m_hitPoints == 0; }

void Monster::onBeginContact(const ContactInfo&) {}
void Monster::onEndContact(const ContactInfo&) {}
void Monster::onPreSolve(const ContactInfo& info, const b2Manifold&)
{
    if (info.getOtherIdentifier()->tag == TAG_PLAYER)
    {
        Player* player =
            static_cast<Player*>(info.getOtherIdentifier()->object);
        player->takeDamge(m_damageWhenTouching);
    }
}
void Monster::onPostSolve(const ContactInfo&, const b2ContactImpulse&) {}
