#include "Level.h"
#include "AssertManager.h"
#include "Background.h"
#include "Builder.h"
#include "CollisionHandler.h"
#include "Constances.h"
#include "Enums.h"
#include "Game.h"
#include "GameObject.h"
#include "HUD.h"
#include "Input.h"
#include "Kobold.h"
#include "LayerManager.h"
#include "Locator.h"
#include "MainState.h"
#include "Math.h"
#include "Player.h"
#include "Rect.h"
#include "SDL_mixer.h"
#include "SDL_mouse.h"
#include "SDL_render.h"
#include "StateManager.h"
#include "Tilesets.h"
#include "Utils.h"
#include "WorldRenderer.h"
#include "box2d/b2_fixture.h"
#include "tmxlite/Map.hpp"
#include "tmxlite/ObjectGroup.hpp"
#include "tmxlite/TileLayer.hpp"
#include "tmxlite/Types.hpp"
#include "WorldManager.h"
#include <cstdlib>
Level::Level()
{
    WorldManager::clearWorld();
    /*
    m_monstersToBeRemovedCount  = 0;
    m_fireballsToBeRemovedCount = 0;
    */
    m_isPaused = false;

    m_player     = nullptr;
    m_viewport.x = 0;
    m_viewport.y = 0;
    m_viewport.w = Constances::GAME_WIDTH;
    m_viewport.h = Constances::GAME_HEIGHT;

    m_worldRenderer =
        new WorldRenderer(Locator::getRenderer(), Constances::PPM);
    m_worldRenderer->AppendFlags(b2Draw::e_shapeBit);
    m_worldRenderer->AppendFlags(b2Draw::e_pairBit);

    b2World* world = WorldManager::getWorld();
    world->SetContactListener(this);
    world->SetDebugDraw(m_worldRenderer);
}

Level::~Level()
{
    delete m_worldRenderer;
    delete m_tileSets;

    m_worldRenderer = nullptr;
    m_tileSets      = nullptr;
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
}

void Level::render()
{
    LayerManager::render();
    m_worldRenderer->setViewport(getCamera().getViewport());
    m_player->paint();
    // m_world->DrawDebugData();
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
