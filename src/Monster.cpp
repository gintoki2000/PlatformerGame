#include "Monster.h"
#include "Adventurer.h"
#include "Constances.h"
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
    if (m_body != nullptr)
    {
        m_body->SetUserData(nullptr);
        m_body->GetWorld()->DestroyBody(m_body);
        m_body = nullptr;
    }
}

bool Monster::Init(const FloatRect& aabb)
{
    b2World&  world = *WorldManager::GetWorld();
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
        CATEGORY_BIT_ADVENTURER | CATEGORY_BIT_BLOCK | CATEGORY_BIT_PROJECTILE;
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

    m_damageWhenTouching = 1;
    return true;
}

void Monster::Tick(float)
{
    m_positionX = m_body->GetPosition().x * Constances::PPM;
    m_positionY = m_body->GetPosition().y * Constances::PPM;
}

bool Monster::TakeDamge(int, Direction) {}

int  Monster::GetHitPoints() { return m_hitPoints; }
int  Monster::GetMaxHitPoints() { return m_maxHitPoints; }
bool Monster::IsDead() { return m_hitPoints == 0; }

void Monster::OnBeginContact(const ContactInfo&) {}
void Monster::OnEndContact(const ContactInfo&) {}
void Monster::OnPreSolve(const ContactInfo& info, const b2Manifold&)
{
    const Identifier* otherIDR = info.GetOtherIdentifier();
    if (otherIDR != nullptr && otherIDR->tag == TAG_ADVENTURER)
    {
        if (!IsDead())
        {
            Adventurer* adventurer = static_cast<Adventurer*>(otherIDR->object);
            Direction   direction =
                RelativeDirection(adventurer->GetPositionX(), GetPositionX());
            if (adventurer->TakeDamge(1, direction) && adventurer->IsGrounded())
            {
                b2Vec2 f;
                f.x = -DirectionToSign(direction) * 20.f;
                f.y = -5.f;
                adventurer->GetBody()->ApplyLinearImpulseToCenter(f, true);
            }
        }
        info.SetIsEnabled(false);
    }
}
void Monster::OnPostSolve(const ContactInfo&, const b2ContactImpulse&) {}
