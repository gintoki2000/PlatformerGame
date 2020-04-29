#include "Level.h"
#include "AnimatedParticle.h"
#include "Background.h"
#include "BloodStainParticle.h"
#include "CameraShaker.h"
#include "CollisionHandler.h"
#include "Constances.h"
#include "FireBustParticle.h"
#include "FireExplosionParticle.h"
#include "Game.h"
#include "GameObject.h"
#include "HUD.h"
#include "Input.h"
#include "LayerManager.h"
#include "Math.h"
#include "MyObjectLayer.h"
#include "MyTileLayer.h"
#include "ObjectFactory.h"
#include "ObjectLayer.h"
#include "ParticleSystem.h"
#include "Player.h"
#include "Pool.h"
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
#include "tmxlite/ImageLayer.hpp"
#include "tmxlite/Layer.hpp"
#include "tmxlite/Map.hpp"
#include "tmxlite/ObjectGroup.hpp"
#include "tmxlite/TileLayer.hpp"
#include "tmxlite/Types.hpp"
#include <cstdlib>
#include <ctime>
Level::Level()
{
    m_spriteLayer   = nullptr;
    m_tilesets      = nullptr;
    m_player        = nullptr;
    m_particleLayer = nullptr;
    m_music         = nullptr;
    m_cameraShaker  = nullptr;
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
    srand(time(nullptr));

    /// load data
    tmx::Map map;
    if (!map.load(filename))
    {
        SDL_Log("Failed to load data: %s", filename);
        return false;
    }

    unsigned rows       = map.getTileCount().y;
    unsigned tileHeight = map.getTileSize().y;
    Vec2     vs(Constances::GAME_WIDTH, Constances::GAME_HEIGHT);
    Vec2     vc;
    vc.x = vs.x / 2;
    vc.y = rows * tileHeight - vs.y / 2.f;
    getCamera().setSize(vs);
    getCamera().setCenter(vc);

    m_tilesets = new Tilesets(map.getTilesets(), GAME->renderer());
    for (const auto& l : map.getLayers())
    {
        tmx::Layer& layerData = dynamic_cast<tmx::Layer&>(*l.get());
        if (layerData.getType() == tmx::Layer::Type::Tile)
        {
            auto& data = dynamic_cast<tmx::TileLayer&>(layerData);
            addLayer(MyTileLayer::create(map, data, m_tilesets));
        }
        else if (layerData.getType() == tmx::Layer::Type::Image)
        {
            auto& data = dynamic_cast<tmx::ImageLayer&>(layerData);
            addLayer(Background::create(data));
        }
        else if (layerData.getType() == tmx::Layer::Type::Object)
        {
            auto& data = dynamic_cast<tmx::ObjectGroup&>(layerData);

            addLayer(MyObjectLayer::create(data));
        }
    }

    m_music = Mix_LoadMUS("asserts/musics/Next to You.mp3");

    SDL_Renderer* renderer = GAME->renderer();
    b2World*      world    = WorldManager::getWorld();

    m_worldRenderer  = new WorldRenderer(renderer, Constances::PPM);
    m_player         = Player::create(getCamera().getCenter());
    m_hud            = HUD::create();
    m_particleSystem = new ParticleSystem(this);
    m_cameraShaker   = CameraShaker::create(&getCamera());
    m_spriteLayer    = static_cast<ObjectLayer*>(getLayerByName("sprites"));
    m_particleLayer  = static_cast<ObjectLayer*>(getLayerByName("particles"));
    m_drawDebugData  = false;

    m_spriteLayer->addObject(m_player);
    world->SetContactListener(this);
    world->SetDebugDraw(m_worldRenderer);
    m_worldRenderer->AppendFlags(b2Draw::e_shapeBit);
    m_worldRenderer->AppendFlags(b2Draw::e_pairBit);
    m_particleSystem
        ->resgiter<FireBustParticle, AnimatedParticlePool<FireBustParticle>>(
            20);
    m_particleSystem
        ->resgiter<BloodStainParticle, BasePool<BloodStainParticle>>(20);
    m_particleSystem->resgiter<FireExplosionParticle,
                               AnimatedParticlePool<FireExplosionParticle>>(20);
    return true;
}

Level::~Level()
{
    DELETE_NULL(m_worldRenderer);
    DELETE_NULL(m_tilesets);
    DELETE_NULL(m_particleSystem);
    DELETE_NULL(m_cameraShaker);
    DELETE_NULL(m_hud);
    Mix_FreeMusic(m_music);
    m_music = nullptr;
}

void Level::start() { Mix_PlayMusic(m_music, -1); }

Player* Level::getPlayer() const { return m_player; }

Tilesets* Level::getTilesets() const { return m_tilesets; }

void Level::setIsPaused(bool paused) { m_isPaused = paused; }

void Level::update(float deltaTime)
{
    WorldManager::getWorld()->Step(deltaTime, 2, 6);
    LayerManager::update(deltaTime);

    Vec2 cameraTarget;
    int  sign      = directionToSign(m_player->getDirection());
    cameraTarget.x = m_player->getPositionX() + sign * 16.f;
    cameraTarget.y = getCamera().getCenter().y;

    int leftBound = Constances::GAME_WIDTH / 2;
    if (cameraTarget.x < leftBound)
    {
        cameraTarget.x = leftBound;
    }
    getCamera().setTarget(cameraTarget);
    m_cameraShaker->tick(deltaTime);
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
    m_hud->paint(*this);
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

ObjectLayer* Level::getParticleLayer() const { return m_particleLayer; }

ObjectLayer* Level::getSpriteLayer() const { return m_spriteLayer; }

CameraShaker* Level::getCameraShaker() { return m_cameraShaker; }

ParticleSystem* Level::getParticleSystem() const { return m_particleSystem; }
