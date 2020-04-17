#include "WorldManager.h"
#include "Constances.h"
#include "box2d/box2d.h"

b2World* WorldManager::world = nullptr;

b2World* WorldManager::getWorld() { return world; }

void WorldManager::clearWorld() { 
	if (world != nullptr)
	{
		delete world;
		world = nullptr;
	}
	world = new b2World(Constances::GRAVITY);
}
