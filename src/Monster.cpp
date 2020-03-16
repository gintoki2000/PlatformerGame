#include "Monster.h"
#include "Constances.h"
#include "Enums.h"
#include "GameObject.h"

Monster::Monster(int monsterType, int hitPoints) :
    GameObject(GAME_OBJECT_TYPE_MONSTER),
    m_monsterType(monsterType),
    m_hitPoints(hitPoints)
{
}

b2Body* Monster::createBody(b2World* world, int left, int top, int width,
                            int height)
{
    b2BodyDef bdef;
    bdef.position.x = (left + width / 2.f) / Constances::PPM;
    bdef.position.y = (top + height / 2.f) / Constances::PPM;
    bdef.fixedRotation = true;
    bdef.type = b2_dynamicBody;

    b2Body* body = world->CreateBody(&bdef);

    b2PolygonShape box;
    box.SetAsBox(width / 2.f / Constances::PPM, height / 2.f / Constances::PPM);

    b2FixtureDef fdef;
    fdef.shape = &box;
    fdef.filter.categoryBits = CATEGORY_BIT_MONSTER;
    fdef.filter.maskBits = CATEGORY_BIT_BLOCK | CATEGORY_BIT_PLAYER;

    body->CreateFixture(&fdef);

    return body;
}
