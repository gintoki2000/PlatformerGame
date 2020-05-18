#include "Level.h"
#include "Adventurer.h"
#include "AnimatedParticle.h"
#include "Background.h"
#include "BloodStainParticle.h"
#include "CameraShaker.h"
#include "CollisionHandler.h"
#include "Constances.h"
#include "EquipmentMenu.h"
#include "FireBustParticle.h"
#include "FireExplosionParticle.h"
#include "Game.h"
#include "GameObject.h"
#include "HUD.h"
#include "Input.h"
#include "Math.h"
#include "MyObjectLayer.h"
#include "MyTileLayer.h"
#include "ObjectFactory.h"
#include "ObjectLayer.h"
#include "ParticleSystem.h"
#include "Pool.h"
#include "Rect.h"
#include "SDL_assert.h"
#include "SDL_keyboard.h"
#include "SDL_mixer.h"
#include "SDL_mouse.h"
#include "SDL_render.h"
#include "Scene.h"
#include "StateManager.h"
#include "TextureManager.h"
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
    m_adventurer    = nullptr;
    m_particleLayer = nullptr;
    m_music         = nullptr;
    m_cameraShaker  = nullptr;
}

Level* Level::LoadFromFile(const char* filename)
{
    Level* level = new Level;
    if (level->Init(filename))
    {
        return level;
    }
    delete level;
    return nullptr;
}

bool Level::Init(const char* filename)
{
    WorldManager::ClearWorld();
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
    GetCamera().SetSize(vs);
    GetCamera().SetCenter(vc);

    m_tilesets = new Tilesets(map.getTilesets(), GAME->GetRenderer());
    for (const auto& l : map.getLayers())
    {
        tmx::Layer& layerData = dynamic_cast<tmx::Layer&>(*l.get());
        if (layerData.getType() == tmx::Layer::Type::Tile)
        {
            auto& data = dynamic_cast<tmx::TileLayer&>(layerData);
            AddLayer(MyTileLayer::Create(map, data, m_tilesets));
        }
        else if (layerData.getType() == tmx::Layer::Type::Image)
        {
            auto& data = dynamic_cast<tmx::ImageLayer&>(layerData);
            AddLayer(Background::Create(data));
        }
        else if (layerData.getType() == tmx::Layer::Type::Object)
        {
            auto& data = dynamic_cast<tmx::ObjectGroup&>(layerData);
            AddLayer(MyObjectLayer::Create(data));
        }
    }

    m_music = Mix_LoadMUS("asserts/musics/Next to You.mp3");

    SDL_Renderer* renderer = GAME->GetRenderer();
    b2World*      world    = WorldManager::GetWorld();

    m_worldRenderer  = new WorldRenderer(renderer, Constances::PPM);
    m_adventurer     = new Adventurer(GetCamera().GetCenter());
    m_hud            = new HUD();
    m_particleSystem = new ParticleSystem(this);
    m_cameraShaker   = CameraShaker::Create(&GetCamera());
    m_spriteLayer    = static_cast<ObjectLayer*>(GetLayerByName("sprites"));
    m_particleLayer  = static_cast<ObjectLayer*>(GetLayerByName("particles"));
    m_pauseMenu      = new EquipmentMenu();
    m_drawDebugData  = false;

    m_spriteLayer->AddObject(m_adventurer);
    world->SetContactListener(this);
    world->SetDebugDraw(m_worldRenderer);
    m_worldRenderer->AppendFlags(b2Draw::e_shapeBit);
    m_worldRenderer->AppendFlags(b2Draw::e_pairBit);

    m_particleSystem->Resgiter<FireBustParticle, FireBustParticle::Pool>(20);
    m_particleSystem->Resgiter<BloodStainParticle, BloodStainParticle::Pool>(
        20);
    AddLayer(m_pauseMenu);
    AddLayer(m_hud);

    m_pauseMenu->Hide();
    return true;
}

Level::~Level()
{
    DELETE_NULL(m_worldRenderer);
    DELETE_NULL(m_tilesets);
    DELETE_NULL(m_particleSystem);
    DELETE_NULL(m_cameraShaker);
    Mix_FreeMusic(m_music);
    m_music = nullptr;
}

void Level::Start()
{
    Mix_PlayMusic(m_music, -1);
    m_state = STATE_PLAYING;
    Scene::Start();
}

Adventurer* Level::GetAdventurer() const { return m_adventurer; }

Tilesets* Level::GetTilesets() const { return m_tilesets; }

void Level::SetIsPaused(bool paused) { m_isPaused = paused; }

void Level::Tick(float deltaTime)
{
    switch (m_state)
    {
    case STATE_EQUIPMENT:
    {
        m_pauseMenu->HandleInput();
    }
    break;
    case STATE_PLAYING:
    {
        if (Input::IsPressed(BUTTON_START))
        {
            DisplayEquipmentMenu();
        }
        else
        {
            m_adventurer->HandleInput();
            m_cameraShaker->Tick(deltaTime);
        }
    }
    }
    WorldManager::GetWorld()->Step(deltaTime, 6, 2);
    Scene::Tick(deltaTime);
    UpdateCamera(deltaTime);
}

void Level::Paint()
{
    Scene::Paint();
    m_worldRenderer->SetViewport(GetCamera().GetViewport());
    if (m_drawDebugData)
    {
        WorldManager::GetWorld()->DrawDebugData();
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
        indentifierA->object->OnPreSolve(info, *oldManifold);
    }
    if (indentifierB != nullptr)
    {
        ContactInfo info(contact, fixtureB, fixtureA, indentifierA);
        indentifierB->object->OnPreSolve(info, *oldManifold);
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
        indentifierA->object->OnBeginContact(info);
    }
    if (indentifierB != nullptr)
    {
        ContactInfo info(contact, fixtureB, fixtureA, indentifierA);
        indentifierB->object->OnBeginContact(info);
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
        indentifierA->object->OnEndContact(info);
    }
    if (indentifierB != nullptr)
    {
        ContactInfo info(contact, fixtureB, fixtureA, indentifierA);
        indentifierB->object->OnEndContact(info);
    }
}

void Level::PostSolve(b2Contact* /*contact*/,
                      const b2ContactImpulse* /*impulse*/)
{
}

ObjectLayer* Level::GetParticleLayer() const { return m_particleLayer; }

ObjectLayer* Level::GetSpriteLayer() const { return m_spriteLayer; }

CameraShaker* Level::GetCameraShaker() { return m_cameraShaker; }

ParticleSystem* Level::GetParticleSystem() const { return m_particleSystem; }

void Level::DisplayEquipmentMenu()
{
    Input::ResetState();
    m_pauseMenu->Show();
    m_pauseMenu->Activate();
    m_state = STATE_EQUIPMENT;
    for (int i = 0; i < GetNumLayers(); ++i)
    {
        Layer* layer = GetLayerAt(i);
        if (layer != m_pauseMenu)
        {
            layer->Deactivate();
            layer->Hide();
        }
    }
}

void Level::HideEquipmentMenu()
{
    Input::ResetState();
    m_pauseMenu->Hide();
    m_pauseMenu->Deactivate();
    m_state = STATE_PLAYING;
    for (int i = 0; i < GetNumLayers(); ++i)
    {
        Layer* layer = GetLayerAt(i);
        if (layer != m_pauseMenu)
        {
            layer->Activate();
            layer->Show();
        }
    }
}

void Level::UpdateCamera(float)
{
    Vec2 cameraTarget;
    int  sign      = DirectionToSign(m_adventurer->GetDirection());
    cameraTarget.x = m_adventurer->GetPositionX() + sign * 16.f;
    cameraTarget.y = GetCamera().GetCenter().y;

    int leftBound = Constances::GAME_WIDTH / 2;
    if (cameraTarget.x < leftBound)
    {
        cameraTarget.x = leftBound;
    }
    GetCamera().SetTarget(cameraTarget);
}
