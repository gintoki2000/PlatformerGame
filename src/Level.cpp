#include "Level.h"
#include "Constances.h"
#include "Game.h"
#include "Player.h"
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
    if ((m_player = Player::create(this)) == nullptr)
    {
        return false;
    }

    createGround();
    createWall();

    m_world->SetDebugDraw(&m_worldRenderer);
    m_worldRenderer.AppendFlags(b2Draw::e_shapeBit);

    m_viewPort = {0, 0, Game::WIDTH, Game::HEIGHT};
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
    delete m_world;
}

void Level::update(float dt)
{
    m_player->update(dt);
    m_world->Step(dt, 2, 6);
}

void Level::draw(SDL_Renderer* renderer)
{
    m_player->draw(renderer, m_viewPort);
    m_world->DrawDebugData();
}

void Level::BeginContact(b2Contact* c) {}

void Level::EndContact(b2Contact* c) {}

void Level::createGround()
{
    b2BodyDef groundBodyDef;
    groundBodyDef.type = b2_staticBody;
    const float GROUND_HEIGHT = 16.f;
    groundBodyDef.position.x = Game::WIDTH / 2.f / Constances::PPM;
    groundBodyDef.position.y =
        (Game::HEIGHT - GROUND_HEIGHT / 2.f) / Constances::PPM;
    b2Body* groundBody = m_world->CreateBody(&groundBodyDef);

    b2FixtureDef groundFixtureDef;
    b2PolygonShape groundShape;
    groundShape.SetAsBox(Game::WIDTH / 2.f / Constances::PPM,
                         GROUND_HEIGHT / 2.f / Constances::PPM);
    groundFixtureDef.shape = &groundShape;

    groundBody->CreateFixture(&groundFixtureDef);
}

void Level::createWall()
{
    int width = 1.f;
    int height = 10.f;
    createBox(width / 2.f, height / 2.f + 10.f, width, height);

	width = 6.f;
	height = 2.f;
	createBox(Game::WIDTH / 2.f / Constances::PPM + 5.f, Game::HEIGHT / 2.f / Constances::PPM + 5.f, width, height);
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
    fDef.friction = 0.2f;
    body->CreateFixture(&fDef);
}
