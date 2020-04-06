#include "Level.h"
#include "AssertManager.h"
#include "Builder.h"
#include "Cell.h"
#include "Constances.h"
#include "Enums.h"
#include "Fireball.h"
#include "Game.h"
#include "GameObject.h"
#include "HUD.h"
#include "Input.h"
#include "Kobold.h"
#include "Locator.h"
#include "MainState.h"
#include "Math.h"
#include "Monster.h"
#include "NTRect.h"
#include "ObjectList.h"
#include "Player.h"
#include "SDL_mixer.h"
#include "SDL_mouse.h"
#include "SDL_render.h"
#include "Slime.h"
#include "StateManager.h"
#include "TiledMap.h"
#include "WorldRenderer.h"
#include "box2d/b2_fixture.h"
#include "tmxlite/Map.hpp"
#include "tmxlite/ObjectGroup.hpp"
#include "tmxlite/TileLayer.hpp"
#include "tmxlite/Types.hpp"
#include <cstdlib>
#include <vector>
Level::Level()
{
    m_world                     = new b2World(Constances::GRAVITY);
    m_monstersToBeRemovedCount  = 0;
    m_fireballsToBeRemovedCount = 0;
    m_isPaused                  = false;
    m_monsters                  = new ObjectList();
    m_tiledMap                  = nullptr;
    m_player                    = nullptr;
    m_viewport.x                = 0;
    m_viewport.y                = 0;
    m_viewport.w                = Constances::GAME_WIDTH;
    m_viewport.h                = Constances::GAME_HEIGHT;
    m_fireballs                 = new ObjectList();
    m_worldRenderer =
        new WorldRenderer(Locator::getRenderer(), Constances::PPM);
    m_world->SetDebugDraw(m_worldRenderer);
    m_world->SetContactListener(this);

    m_worldRenderer->AppendFlags(b2Draw::e_shapeBit);
    m_textureManager = new TextureManager(Locator::getRenderer());
}

Level::~Level()
{
    delete m_monsters;
    delete m_world;
    delete m_tiledMap;
    delete m_worldRenderer;
    delete m_fireballs;
    Mix_FreeMusic(m_music);
    m_music = nullptr;
    for (auto t : m_tileSets)
    {
        delete t;
    }
    m_tileSets.clear();
    delete m_textureManager;
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
    std::string textures[] = {
        "asserts/spritesheets/player.png", "asserts/spritesheets/slime.png",
        "asserts/spritesheets/kobold.png", "asserts/spritesheets/fire-ball.png",
        "asserts/spritesheets/potions-sheet.png"};
    for (const auto& texture : textures)
    {
        if (!m_textureManager->load(texture))
        {
            SDL_Log("Failed to load asserts: %s", texture.c_str());
            return false;
        }
    }
    if ((m_music = Mix_LoadMUS("asserts/musics/The Last Encounter.wav")) ==
        nullptr)
    {
        SDL_Log("Failed to load music: %s", Mix_GetError());
        return false;
    }
    /// load level  data
    tmx::Map levelData;
    if (!levelData.load(filename))
    {
        SDL_Log("Failed to load file: %s", filename.c_str());
        return false;
    }
    /// load tilesets data
    m_tileSets.reserve(levelData.getTilesets().size());
    for (const auto& tileSetData : levelData.getTilesets())
    {
        m_tileSets.push_back(new Tileset(tileSetData, Locator::getRenderer()));
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
    m_viewportX = 0;
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
    m_player = new Player(this);
    Mix_PlayMusic(m_music, -1);
    m_HUD = new HUD(this);
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
            m_fireballs->tick(deltaTime);
        }
        for (int i = 0; i < m_monstersToBeRemovedCount; ++i)
        {
            m_monsters->removeObject(m_monstersToBeRemoved[i]);
        }
        for (int i = 0; i < m_fireballsToBeRemovedCount; ++i)
        {
            m_fireballs->removeObject(m_fireballsToBeRemoved[i]);
        }

        m_fireballsToBeRemovedCount = 0;
        m_monstersToBeRemovedCount  = 0;
    }
    updateViewport(deltaTime);
    m_worldRenderer->setViewport(m_viewport);

    int    x, y;
    Uint32 mouseState = SDL_GetMouseState(&x, &y);
    /*
    if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
        auto slime = new Slime(this);
        slime->setPositionX(x / Constances::SCALE_X + m_viewport.x);
        slime->setPositionY(y / Constances::SCALE_Y + m_viewport.y);
        addMonster(slime);
    }*/

    if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT))
    {
        auto kobold = new Koblod(this);
        kobold->setPositionX(x / Constances::SCALE_X + m_viewport.x);
        kobold->setPositionY(y / Constances::SCALE_Y + m_viewport.y);
        addMonster(kobold);
    }
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
    m_fireballs->paint();
    m_HUD->paint();
    m_world->DrawDebugData();
}

void Level::addMonster(Monster* monster) { m_monsters->addObject(monster); }

void Level::removeMonster(Monster* monster)
{
    SDL_assert(m_monstersToBeRemovedCount < MAX_SIZE);
    m_monstersToBeRemoved[m_monstersToBeRemovedCount++] = monster;
}

const Rect& Level::getViewport() const { return m_viewport; }

b2World* Level::getWorld() const { return m_world; }

Player* Level::getPlayer() const { return m_player; }

const std::vector<Tileset*>& Level::getTilesets() const { return m_tileSets; }

void Level::setPaused(bool paused) { m_isPaused = paused; }

void Level::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
}

void Level::BeginContact(b2Contact* contact)
{
    b2Fixture*  fixtureA = contact->GetFixtureA();
    b2Fixture*  fixtureB = contact->GetFixtureB();
    GameObject* objectA  = (GameObject*)fixtureA->GetBody()->GetUserData();
    GameObject* objectB  = (GameObject*)fixtureB->GetBody()->GetUserData();
    if (objectA != nullptr &&
        objectA->getGameObjectType() == GAME_OBJECT_TYPE_SPELL)
    {
        if (objectB != nullptr &&
            objectB->getGameObjectType() == GAME_OBJECT_TYPE_MONSTER)
        {
            //Fireball* fireball = (Fireball*)objectA;
            Monster*  monster  = (Monster*)objectB;
            if (!monster->isDead())
            {
                monster->getHit(1);
            }
        }
    }
    if (objectA != nullptr &&
        objectA->getGameObjectType() == GAME_OBJECT_TYPE_MONSTER)
    {
        if (objectB != nullptr &&
            objectB->getGameObjectType() == GAME_OBJECT_TYPE_SPELL)
        {
            //Fireball* fireball = (Fireball*)objectB;
            Monster*  monster  = (Monster*)objectA;
            if (!monster->isDead())
            {
                monster->getHit(1);
            }
        }
    }
}
void Level::EndContact(b2Contact*) {}

TextureManager* Level::getTextureManager() const { return m_textureManager; }

void Level::updateViewport(float)
{
    int   sign    = m_player->getDirection() == DIRECTION_LEFT ? -1 : 1;
    float targetX = m_player->getPositionX() - m_viewport.w / 2 + sign * 16.f;
    float levelWidth = m_tiledMap->getWidth() * m_tiledMap->getTileWidth();
    m_viewportX += Math::lerp(m_viewportX, targetX, 0.1f);

    m_viewport.x = Math::clamp(0.f, levelWidth - m_viewport.w, m_viewportX);
}

void Level::addFireball(Fireball* fireball)
{
    m_fireballs->addObject(fireball);
}

void Level::removeFireball(Fireball* fireball)
{
    m_fireballsToBeRemoved[m_fireballsToBeRemovedCount++] = fireball;
}
