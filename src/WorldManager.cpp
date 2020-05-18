#include "WorldManager.h"
#include "Constances.h"
#include "Utils.h"
#include "box2d/box2d.h"

b2World* WorldManager::world = nullptr;

b2World* WorldManager::GetWorld() { return world; }

void WorldManager::ClearWorld()
{
    if (world != nullptr)
    {
        delete world;
        world = nullptr;
    }
    world = new b2World(Constances::GRAVITY);
}

void WorldManager::Terminate() { DELETE_NULL(world); }
