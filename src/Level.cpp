
#include "Level.h"
#include "Builder.h"
#include "Cell.h"
#include "Constances.h"
#include "Enums.h"
#include "Input.h"
#include "Locator.h"
#include "Monster.h"
#include "NTRect.h"
#include "ObjectList.h"
#include "Player.h"
#include "SDL_render.h"
#include "Slime.h"
#include "TiledMap.h"
#include "WorldRenderer.h"
#include "tmxlite/Map.hpp"
#include "tmxlite/ObjectGroup.hpp"
#include "tmxlite/TileLayer.hpp"
#include "tmxlite/Types.hpp"
#include <algorithm>
#include <cstdlib>
#include <vector>
Level::Level()
{
    m_world                    = new b2World(Constances::GRAVITY);
    m_monstersToBeRemovedCount = 0;
    m_isPaused                 = false;
    m_monsters                 = new ObjectList();
    m_tiledMap                 = nullptr;
    m_player                   = nullptr;
    m_viewport.x               = 0;
    m_viewport.y               = 0;
    m_viewport.w               = Constances::GAME_WIDTH;
    m_viewport.h               = Constances::GAME_HEIGHT;
    m_worldRenderer =
        new WorldRenderer(Locator::getRenderer(), Constances::PPM);
    m_world->SetDebugDraw(m_worldRenderer);
    m_world->SetContactListener(this);

    m_worldRenderer->AppendFlags(b2Draw::e_shapeBit);
}

Level::~Level()
{
    delete m_monsters;
    delete m_world;
    delete m_tiledMap;
    delete m_worldRenderer;
    for (auto t : m_tileSets)
    {
        delete t;
    }
    m_tileSets.clear();
}

Level* Level::create(const std::string& filename)
{
    Level* ret = new Level;
    if (ret->init(filename))
    {
        return ret;
    }
    delete ret;
    return nullptr;
}

static std::vector<tmx::Layer::Ptr>::const_iterator
findLayer(const std::vector<tmx::Layer::Ptr>& layers,
          const std::string&                  layerName)
{
    return std::find_if(std::begin(layers), std::end(layers),
                        [&](const tmx::Layer::Ptr& layer) -> bool {
                            return layer->getName() == layerName;
                        });
}

bool Level::init(const std::string& filename)
{
    /// load level  data
    tmx::Map levelData;
    if (!levelData.load(filename))
    {
        SDL_Log("Loading level failed!");
        return false;
    }
    /// load tilesets data
    m_tileSets.reserve(levelData.getTilesets().size());
    for (const auto& tileSetData : levelData.getTilesets())
    {
        m_tileSets.push_back(new TileSet(tileSetData, Locator::getRenderer()));
    }

    /// create soild tiledmap
    const auto& layers     = levelData.getLayers();
    auto        findResult = findLayer(layers, "tiledmap");
    if (findResult == std::end(layers))
    {
        SDL_Log("Invalid level data!");
        return false;
    }
    tmx::TileLayer* tileLayerData = (tmx::TileLayer*)findResult->get();
    m_tiledMap = new TiledMap(this, levelData, *tileLayerData);

    m_viewport.y = m_tiledMap->getHeight() * m_tiledMap->getTileHeight() -
                   Constances::GAME_HEIGHT;

    /// create ground
    auto findResult2 = findLayer(layers, "solid-objects");
    if (findResult == std::end(layers))
    {
        SDL_Log("Invalid level data!");
        return false;
    }
    auto solidObjects = (tmx::ObjectGroup*)findResult2->get();

    for (const auto& solidObject : solidObjects->getObjects())
    {

        b2BodyDef blockbdef;
        blockbdef.fixedRotation = true;
        blockbdef.type          = b2_staticBody;
        blockbdef.position.x    = solidObject.getPosition().x / Constances::PPM;
        blockbdef.position.y    = solidObject.getPosition().y / Constances::PPM;

        b2FixtureDef blockfdef;
        blockfdef.shape               = Builder::buildShape(solidObject);
        blockfdef.filter.categoryBits = CATEGORY_BIT_BLOCK;
        m_world->CreateBody(&blockbdef)->CreateFixture(&blockfdef);
        delete blockfdef.shape;
    }
    auto slime = Slime::create(this, 100.f, 100.f);
    m_monsters->addObject(slime);
    if ((m_player = Player::create(this)) == nullptr)
    {
        SDL_Log("Failed to create player!");
        return false;
    }
    m_player->setPosition(100.f, 10.f);
    return true;
}

void Level::tick(float deltaTime)
{
    Input::update();
    if (!m_isPaused)
    {
        m_world->Step(deltaTime, 2, 6);
        if (!m_isPaused)
        {
            m_monsters->tick(deltaTime);
            m_tiledMap->tick(deltaTime);
            m_player->tick(deltaTime);
        }
        for (int i = 0; i < m_monstersToBeRemovedCount; ++i)
        {
            m_monsters->removeObject(m_monstersToBeRemoved[i]);
        }
    }
    m_worldRenderer->setViewport(m_viewport);
}

void Level::render(float deltaTime)
{

    tick(deltaTime);
    SDL_SetRenderDrawColor(Locator::getRenderer(), 0x00, 0x00, 0x00, 0xff);
    SDL_RenderClear(Locator::getRenderer());
    m_monstersToBeRemovedCount = 0;
    m_tiledMap->paint();
    m_monsters->paint();
    m_player->paint();
    m_world->DrawDebugData();
}

void Level::addMonster(Monster* monster) { m_monsters->addObject(monster); }

void Level::removeMonster(Monster* monster)
{
    SDL_assert(m_monstersToBeRemovedCount < MAX_SIZE);
    m_monstersToBeRemoved[m_monstersToBeRemovedCount++] = monster;
}

const NTRect& Level::getViewport() const { return m_viewport; }

b2World* Level::getWorld() const { return m_world; }

Player* Level::getPlayer() const { return m_player; }

const std::vector<TileSet*>& Level::getTilesets() const { return m_tileSets; }

void Level::setPaused(bool paused) { m_isPaused = paused; }

void Level::BeginContact(b2Contact* contact)
{
    b2Fixture* fixtureA  = contact->GetFixtureA();
    b2Fixture* fixtureB  = contact->GetFixtureB();
    void*      userDataA = fixtureA->GetBody()->GetUserData();
    void*      userDataB = fixtureB->GetBody()->GetUserData();
    if (userDataA == m_player)
    {
        if ((long)fixtureA->GetUserData() == Player::FIXTURE_TYPE_FOOT_SENSOR)
        {
            m_player->touchGround();
        }
    }
    if (userDataB == m_player)
    {
        if ((long)fixtureB->GetUserData() == Player::FIXTURE_TYPE_FOOT_SENSOR)
        {
            m_player->touchGround();
        }
    }
}

void Level::EndContact(b2Contact* contact)
{

    b2Fixture* fixtureA  = contact->GetFixtureA();
    b2Fixture* fixtureB  = contact->GetFixtureB();
    void*      userDataA = fixtureA->GetBody()->GetUserData();
    void*      userDataB = fixtureB->GetBody()->GetUserData();
    if (userDataA == m_player)
    {
        if ((long)fixtureA->GetUserData() == Player::FIXTURE_TYPE_FOOT_SENSOR)
        {
            m_player->untouchGround();
        }
    }
    if (userDataB == m_player)
    {
        if ((long)fixtureB->GetUserData() == Player::FIXTURE_TYPE_FOOT_SENSOR)
        {
            m_player->untouchGround();
        }
    }
}
