#include "Level.h"
#include "AssertManager.h"
#include "Background.h"
#include "BoarWarrior.h"
#include "Cell.h"
#include "CollisionHandler.h"
#include "Constances.h"
#include "Game.h"
#include "GameObject.h"
#include "HUD.h"
#include "Input.h"
#include "LayerManager.h"
#include "Math.h"
#include "ObjectFactory.h"
#include "ObjectLayer.h"
#include "Player.h"
#include "Rect.h"
#include "SDL_assert.h"
#include "SDL_keyboard.h"
#include "SDL_mixer.h"
#include "SDL_mouse.h"
#include "SDL_render.h"
#include "StateManager.h"
#include "TileLayer.h"
#include "Tilesets.h"
#include "Utils.h"
#include "WorldManager.h"
#include "WorldRenderer.h"
#include "box2d/b2_fixture.h"
#include "tmxlite/Layer.hpp"
#include "tmxlite/Map.hpp"
#include "tmxlite/ObjectGroup.hpp"
#include "tmxlite/TileLayer.hpp"
#include "tmxlite/Types.hpp"
Level::Level()
{
    m_itemLayer  = nullptr;
    m_enemyLayer = nullptr;
    m_tileSets   = nullptr;
    m_player     = nullptr;
    m_particles  = nullptr;
}

Level* Level::loadFromFile(const char* filename)
{
    Level* level = new Level;
    if (level->init(filename))
    {
        return level;
    }
    delete level;
    return nullptr;
}

bool Level::init(const char* filename)
{
    WorldManager::clearWorld();

    /// load data
    tmx::Map levelData;
    if (!levelData.load(filename))
    {
        SDL_Log("Failed to load data: ", filename);
        return false;
    }

    m_rows       = levelData.getTileCount().y;
    m_cols       = levelData.getTileCount().x;
    m_tileWidth  = levelData.getTileSize().x;
    m_tileHeight = levelData.getTileSize().y;
    Vec2 vs(Constances::GAME_WIDTH, Constances::GAME_HEIGHT);
    Vec2 vc;
    vc.x = vs.x / 2;
    vc.y = m_rows * m_tileHeight - vs.y / 2.f;
    getCamera().setSize(vs);
    getCamera().setCenter(vc);

    for (const auto& tileData : levelData.getTilesets())
    {
    }
    m_tileSets = new Tilesets(levelData.getTilesets(), Game::getInstance()->renderer());
    for (const auto& l : levelData.getLayers())
    {
        tmx::Layer& layerData = (tmx::Layer&)*l.get();
        if (layerData.getType() == tmx::Layer::Type::Tile)
        {
            TileLayer* tileLayer =
                new TileLayer(m_cols, m_rows, m_tileWidth, m_tileHeight);
            addLayer(tileLayer);
            parseTileLayer(*tileLayer, (tmx::TileLayer&)layerData);
        }
        else if (layerData.getType() == tmx::Layer::Type::Image)
        {
            Background* background = new Background();
            addLayer(background);
            background->parse((tmx::ImageLayer&)layerData);
        }
        else if (layerData.getType() == tmx::Layer::Type::Object)
        {
            ObjectLayer* objectLayer = new ObjectLayer();
            addLayer(objectLayer);
            parseObjectLayer(*objectLayer, (tmx::ObjectGroup&)layerData);
        }
    }
    SDL_Renderer* renderer = Game::getInstance()->renderer();
    m_worldRenderer        = new WorldRenderer(renderer, Constances::PPM);
    m_worldRenderer->AppendFlags(b2Draw::e_shapeBit);
    m_worldRenderer->AppendFlags(b2Draw::e_pairBit);

    b2World* world = WorldManager::getWorld();
    world->SetContactListener(this);
    world->SetDebugDraw(m_worldRenderer);
    m_player = new Player();
    m_player->setLayerManager(this);
    m_drawDebugData = true;
    return true;
}

Level::~Level()
{
    DELETE_NULL(m_worldRenderer);
    DELETE_NULL(m_tileSets);
    DELETE_NULL(m_player);
}

void Level::start()
{
    m_enemyLayer = (ObjectLayer*)getLayerByName("enemies");
    m_itemLayer  = (ObjectLayer*)getLayerByName("items");
    m_particles  = (ObjectLayer*)getLayerByName("particles");

}

Player* Level::getPlayer() const { return m_player; }

Tilesets* Level::getTilesets() const { return m_tileSets; }

void Level::setIsPaused(bool paused) { m_isPaused = paused; }

void Level::update(float deltaTime)
{
    Input::update();
    WorldManager::getWorld()->Step(deltaTime, 2, 6);
    Vec2 cameraTarget;
    int  sign      = m_player->getDirection() == DIRECTION_LEFT ? -1 : 1;
    cameraTarget.x = m_player->getPositionX() + sign * 16.f;
    cameraTarget.y = getCamera().getCenter().y;
    getCamera().setTarget(cameraTarget);
    m_player->tick(deltaTime);
    LayerManager::update(deltaTime);

    const Uint8* keyStates = SDL_GetKeyboardState(nullptr);
    if (keyStates[SDL_SCANCODE_P])
    {
        m_drawDebugData = !m_drawDebugData;
    }
}

void Level::render()
{
    LayerManager::render();
    m_worldRenderer->setViewport(getCamera().getViewport());
    m_player->paint();
    if (m_drawDebugData)
    {
        WorldManager::getWorld()->DrawDebugData();
    }
}
void Level::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    b2Fixture*  fixtureA = contact->GetFixtureA();
    b2Fixture*  fixtureB = contact->GetFixtureB();
    Identifier* indentifierA =
        static_cast<Identifier*>(fixtureA->GetBody()->GetUserData());
    Identifier* indentifierB =
        static_cast<Identifier*>(fixtureB->GetBody()->GetUserData());

    if (indentifierA != nullptr)
    {
        ContactInfo info(contact, fixtureA, fixtureB, indentifierB);
        indentifierA->object->onPreSolve(info, *oldManifold);
    }
    if (indentifierB != nullptr)
    {
        ContactInfo info(contact, fixtureB, fixtureA, indentifierA);
        indentifierB->object->onPreSolve(info, *oldManifold);
    }
}

void Level::BeginContact(b2Contact* contact)
{
    b2Fixture*  fixtureA = contact->GetFixtureA();
    b2Fixture*  fixtureB = contact->GetFixtureB();
    Identifier* indentifierA =
        static_cast<Identifier*>(fixtureA->GetBody()->GetUserData());
    Identifier* indentifierB =
        static_cast<Identifier*>(fixtureB->GetBody()->GetUserData());

    if (indentifierA != nullptr)
    {
        ContactInfo info(contact, fixtureA, fixtureB, indentifierB);
        indentifierA->object->onBeginContact(info);
    }
    if (indentifierB != nullptr)
    {
        ContactInfo info(contact, fixtureB, fixtureA, indentifierA);
        indentifierB->object->onBeginContact(info);
    }
}
void Level::EndContact(b2Contact* contact)
{
    b2Fixture*  fixtureA = contact->GetFixtureA();
    b2Fixture*  fixtureB = contact->GetFixtureB();
    Identifier* indentifierA =
        static_cast<Identifier*>(fixtureA->GetBody()->GetUserData());
    Identifier* indentifierB =
        static_cast<Identifier*>(fixtureB->GetBody()->GetUserData());

    if (indentifierA != nullptr)
    {
        ContactInfo info(contact, fixtureA, fixtureB, indentifierB);
        indentifierA->object->onEndContact(info);
    }
    if (indentifierB != nullptr)
    {
        ContactInfo info(contact, fixtureB, fixtureA, indentifierA);
        indentifierB->object->onEndContact(info);
    }
}

void Level::PostSolve(b2Contact* /*contact*/,
                      const b2ContactImpulse* /*impulse*/)
{
}

bool Level::parseTileLayer(TileLayer&            tileLayer,
                           const tmx::TileLayer& tileLayerData)
{
    const auto& dataOfTiles = tileLayerData.getTiles();
    for (int y = 0; y < m_rows; ++y)
    {
        for (int x = 0; x < m_cols; ++x)
        {
            const auto& tileData = dataOfTiles[(unsigned)y * m_cols + x];
            Tile*       tile     = m_tileSets->getTile(tileData.ID);
            if (tile != nullptr)
            {
                Vec2 center(x * m_tileWidth, y * m_tileHeight);
                tileLayer.setCellAt(x, y, Cell::create(tile, center));
            }
        }
    }
	tileLayer.setName(tileLayerData.getName());
    return true;
}

bool Level::parseObjectLayer(ObjectLayer&            objectLayer,
                             const tmx::ObjectGroup& objectLayerData)
{
    ObjectFactory& factory = *ObjectFactory::getInstance();
    for (const auto& objectData : objectLayerData.getObjects())
    {
        GameObject* object =
            factory.createObject(objectData.getType(), objectData);
        if (object != nullptr)
        {
            objectLayer.addObject(object);
        }
    }
    objectLayer.setName(objectLayerData.getName());
    return true;
}

ObjectLayer* Level::getParticleLayer() const { return m_particles; }
