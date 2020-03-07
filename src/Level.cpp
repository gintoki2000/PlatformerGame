#include "Level.h"
#include "Constances.h"
#include "Enums.h"
#include "Game.h"
#include "GameObject.h"
#include "Input.h"
#include "Monster.h"
#include "ObjectLayer.h"
#include "Player.h"
#include "Slime.h"
#include "box2d/box2d.h"
Level* Level::create(const std::string& filename)
{
    Level* ret = new Level;
    if (ret && ret->initialize(filename))
    {
        return ret;
    }
    delete ret;
    return nullptr;
}

bool Level::initialize(const std::string&)
{
    m_world = new b2World(Constances::GRAVITY);

    if ((m_slimePool = Slime::Pool::create(10)) == nullptr)
    {
        SDL_Log("Failed to create Slime Pool!");
        return false;
    }

    for (int i = 0; i < 10; ++i)
    {
        m_monstersToBeAdd[i] = nullptr;
        m_monstersToBeRemove[i] = nullptr;
    }

    m_numMonstersToBeAdd = 0;
    m_numMonstersToBeRemove = 0;

    m_monsters = new ObjectLayer();

    m_monsters->addObject(
        m_slimePool->getObject()->initialize(this, {5.f, 7.f}));
    m_monsters->addObject(
        m_slimePool->getObject()->initialize(this, {10.f, 10.f}));
    if ((m_player = Player::create(this)) == nullptr)
    {
        return false;
    }

    createGround();
    createWall();

    m_world->SetDebugDraw(&m_worldRenderer);
    m_world->SetContactListener(this);
    m_worldRenderer.AppendFlags(b2Draw::e_shapeBit);

    m_viewPort = {0, 0, Constances::GAME_WIDTH, Constances::GAME_HEIGHT};
    return true;
}

Level::Level() :
    m_world(nullptr),
    m_worldRenderer(Game::getInstance()->getRenderer(), Constances::PPM)
{
}

Level::~Level()
{
    delete m_player;
    delete m_monsters;
    delete m_world;
    delete m_slimePool;
}

void Level::update(float dt)
{
    for (int i = 0; i < m_numMonstersToBeRemove; ++i)
    {
        m_monsters->removeObject(m_monstersToBeRemove[i]);
        m_monstersToBeRemove[i] = nullptr;
    }
    if (m_monsters->getNumObjects() < 2)
    {
        m_monsters->addObject(
            m_slimePool->getObject()->initialize(this, {15.f, 10.f}));
    }
    m_numMonstersToBeRemove = 0;
    Input::update();
    m_player->update(dt);
    m_monsters->update(dt);
    m_world->Step(dt, 2, 6);
}

void Level::draw(SDL_Renderer* renderer)
{
    m_monsters->draw(renderer, m_viewPort);
    m_player->draw(renderer, m_viewPort);
    m_world->DrawDebugData();
}

void Level::BeginContact(b2Contact* contact)
{

    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    if (fixtureA->GetBody()->GetUserData() != nullptr)
    {
        GameObject* gameObjectA =
            (GameObject*)fixtureA->GetBody()->GetUserData();

        GameObject* gameObjectB =
            (GameObject*)fixtureB->GetBody()->GetUserData();
        switch (gameObjectA->getObjectType())
        {
        case GameObject::Type::PLAYER:
        {
            switch ((long)fixtureA->GetUserData())
            {
            case Player::FIXTURE_TYPE_FOOT_SENSOR:
            {
                ((Player*)gameObjectA)->touchGround();
                break;
            }
            case Player::FIXTURE_TYPE_MAIN_BODY:
            {
                if (gameObjectB != nullptr)
                {
                    handleCollision((Player*)gameObjectA, gameObjectB);
                }
                break;
            }
            case Player::FIXTURE_TYPE_WALL_SENSOR:
            {
                ((Player*)gameObjectA)->touchWall();
            }
            }
            break;
        }
        default:
        {
            if (gameObjectB != nullptr &&
                gameObjectB->getObjectType() == GameObject::Type::PLAYER)
            {
                handleCollision((Player*)gameObjectB, gameObjectA);
            }
        }
        }
    }
}

void Level::EndContact(b2Contact* contact)
{
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    if (fixtureA->GetBody()->GetUserData() != nullptr)
    {
        GameObject* gameObjectA =
            (GameObject*)fixtureA->GetBody()->GetUserData();
        if (gameObjectA->getObjectType() == GameObject::Type::PLAYER)
        {
            if ((long)fixtureA->GetUserData() ==
                Player::FIXTURE_TYPE_FOOT_SENSOR)
            {
                ((Player*)gameObjectA)->untouchGround();
            }

            if ((long)fixtureA->GetUserData() ==
                Player::FIXTURE_TYPE_WALL_SENSOR)
            {
                ((Player*)gameObjectA)->untouchWall();
            }
        }
    }
    if (fixtureB->GetBody()->GetUserData() != nullptr)
    {
        GameObject* gameObjectB =
            (GameObject*)fixtureB->GetBody()->GetUserData();
        if (gameObjectB->getObjectType() == GameObject::Type::PLAYER)
        {
            if ((long)fixtureB->GetUserData() ==
                Player::FIXTURE_TYPE_FOOT_SENSOR)
            {
                ((Player*)gameObjectB)->untouchGround();
            }
            if ((long)fixtureB->GetUserData() ==
                Player::FIXTURE_TYPE_WALL_SENSOR)
            {
                ((Player*)gameObjectB)->untouchWall();
            }
        }
    }
}

void Level::handleCollision(Player* player, GameObject* gameObject)
{
    switch (gameObject->getObjectType())
    {
    case GameObject::Type::MONSTER:
    {
        Monster* monster = (Monster*)gameObject;
        monster->getDamage(1);
        break;
    }
    }
}

void Level::createGround()
{
    b2BodyDef groundBodyDef;
    groundBodyDef.type = b2_staticBody;
    const float GROUND_HEIGHT = 16.f;
    groundBodyDef.position.x = Constances::GAME_WIDTH / 2.f / Constances::PPM;
    groundBodyDef.position.y =
        (Constances::GAME_HEIGHT - GROUND_HEIGHT / 2.f) / Constances::PPM;
    b2Body* groundBody = m_world->CreateBody(&groundBodyDef);

    b2FixtureDef groundFixtureDef;
    b2PolygonShape groundShape;
    groundShape.SetAsBox(Constances::GAME_WIDTH / 2.f / Constances::PPM,
                         GROUND_HEIGHT / 2.f / Constances::PPM);
    groundFixtureDef.shape = &groundShape;
    groundFixtureDef.filter.categoryBits = CATEGORY_BIT_BLOCK;
    groundFixtureDef.filter.maskBits =
        CATEGORY_BIT_PLAYER | CATEGORY_BIT_MONSTER;

    groundBody->CreateFixture(&groundFixtureDef);
}

void Level::createWall()
{
    int width = 1.f;
    int height = 10.f;
    createBox(width / 2.f, height / 2.f + 10.f, width, height);

    width = 6.f;
    height = 2.f;
    createBox(Constances::GAME_WIDTH / 2.f / Constances::PPM + 5.f,
              Constances::GAME_HEIGHT / 2.f / Constances::PPM + 5.f, width,
              height);
}

void Level::createBox(float x, float y, float width, float height)
{
    b2BodyDef bDef;
    bDef.type = b2_staticBody;
    bDef.position.x = x;
    bDef.position.y = y;
    b2Body* body = m_world->CreateBody(&bDef);
    b2FixtureDef fDef;
    b2PolygonShape box;
    box.SetAsBox(width / 2.f, height / 2.f);
    fDef.shape = &box;
    fDef.friction = 0.9f;
    fDef.filter.categoryBits = CATEGORY_BIT_BLOCK;
    body->CreateFixture(&fDef);
}

void Level::removeMonster(Monster* monster)
{
    m_monstersToBeRemove[m_numMonstersToBeRemove++] = monster;
}
