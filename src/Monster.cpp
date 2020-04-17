#include "Monster.h"
#include "Constances.h"
#include "Enums.h"
#include "WorldManager.h"
#include "box2d/b2_world.h"

Monster::Monster() { m_body = nullptr; }

bool Monster::init(const Rect& aabb)
{
    b2World&  world = *WorldManager::getWorld();
    b2BodyDef bdef;
    bdef.fixedRotation = true;
    bdef.position.x    = aabb.left() + aabb.width() / 2;
    bdef.position.y    = aabb.top() + aabb.height() / 2;
    bdef.position.x /= Constances::PPM;
    bdef.position.y /= Constances::PPM;

    m_body = world.CreateBody(&bdef);

    b2PolygonShape box;
    box.SetAsBox(aabb.width() / 2, aabb.height() / 2);

    b2FixtureDef fdef;
    fdef.filter.categoryBits = CATEGORY_BIT_MONSTER;
    fdef.filter.maskBits     = CATEGORY_BIT_PLAYER | CATEGORY_BIT_BLOCK;
    fdef.restitution         = 0;
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
void Monster::onPreSolve(const ContactInfo&, const b2Manifold&) {}
void Monster::onPostSolve(const ContactInfo&, const b2ContactImpulse&) {}
