#include "Level.h"
#include "AssertManager.h"
#include "Constances.h"
#include "Enums.h"
#include "Locator.h"
#include "Monster.h"
#include "NTRect.h"
#include "ObjectList.h"
#include "SDL_render.h"
#include "TiledMap.h"
#include "box2d/b2_polygon_shape.h"
#include "tmxlite/Map.hpp"
#include "tmxlite/ObjectGroup.hpp"
#include "tmxlite/TileLayer.hpp"
#include "tmxlite/Types.hpp"
#include <algorithm>
#include <vector>
Level::Level()
{
    m_assertManager = new AssertManager();
    m_world = new b2World(Constances::GRAVITY);
    m_monstersToBeRemovedCount = 0;
    m_isPaused = false;
    m_monsters = new ObjectList();
    m_tiledMap = nullptr;
    m_player = nullptr;

    // TextureHandler* textureHandler = new
    // TextureHandler(Locator::getRenderer()); AssertFactory*  textureFactory =
    // new AssertFactory(textureHandler);
    // m_assertManager->registerFactory<SDL_Texture>(textureFactory);
}

Level::~Level()
{
    delete m_assertManager;
    delete m_world;
    delete m_monsters;
    delete m_tiledMap;
    for (auto t : m_tileSets)
    {
        delete t;
    }
    m_tileSets.clear();
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

static void buildPolygon(b2PolygonShape&                   polygon,
                         const std::vector<tmx::Vector2f>& points)
{
    b2Vec2* v = new b2Vec2[points.size()];
    for (std::size_t i = 0; i < points.size(); ++i)
    {
        v[i].Set(points[i].x, points[i].y);
    }
    polygon.Set(v, points.size());
    delete[] v;
}

bool Level::init(const std::string& filename)
{
    /// load asserts
    // m_assertManager->loasAssert<SDL_Texture>("asserts/player-2.png");
    // m_assertManager->loasAssert<SDL_Texture>("asserts/slime.png");
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
    const auto& layers = levelData.getLayers();
    auto        findResult = findLayer(layers, "tiledmap");
    if (findResult == std::end(layers))
    {
        SDL_Log("Invalid level data!");
        return false;
    }
    tmx::TileLayer* tileLayerData = (tmx::TileLayer*)findResult->get();
    m_tiledMap = new TiledMap(this, levelData, *tileLayerData);

	/*
    /// create ground
    auto findResult2 = findLayer(layers, "solid-blocks");
    if (findResult == std::end(layers))
    {
        SDL_Log("Invalid level data!");
        return false;
    }
    auto solidBlocks = (tmx::ObjectGroup*)findResult2->get();

    b2BodyDef blockbdef;
    blockbdef.fixedRotation = true;
    blockbdef.type = b2_staticBody;

    b2PolygonShape blockbox;

    b2FixtureDef blockfdef;
    blockfdef.shape = &blockbox;
    blockfdef.filter.categoryBits = CATEGORY_BIT_BLOCK;

    for (const auto& object : solidBlocks->getObjects())
    {
        buildPolygon(blockbox, object.getPoints());
        m_world->CreateBody(&blockbdef)->CreateFixture(&blockfdef);
    }
	*/

    return true;
}

void Level::render(float deltaTime)
{

    if (!m_isPaused)
    {
        m_monsters->tick(deltaTime);
        m_tiledMap->tick(deltaTime);
    }
    for (int i = 0; i < m_monstersToBeRemovedCount; ++i)
    {
        m_monsters->removeObject(m_monstersToBeRemoved[i]);
    }
    m_monstersToBeRemovedCount = 0;
    m_tiledMap->paint();
    m_monsters->paint();
}

void Level::addMonster(Monster* monster) { m_monsters->addObject(monster); }

void Level::removeMonster(Monster* monster)
{
    m_monstersToBeRemoved[m_monstersToBeRemovedCount++] = monster;
}

AssertManager* Level::getAssertManager() const { return m_assertManager; }

const NTRect& Level::getViewport() const { return m_viewport; }

b2World* Level::getWorld() const { return m_world; }

Player* Level::getPlayer() const { return m_player; }

const std::vector<TileSet*>& Level::getTilesets() const { return m_tileSets; }

void Level::setPaused(bool paused) {}
