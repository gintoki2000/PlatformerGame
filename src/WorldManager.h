#ifndef WORLD_MANAGER_H
#define WORLD_MANAGER_H
class b2World;
class WorldManager
{
  public:
    static b2World* getWorld();
    static void     clearWorld();
	static void 	end();

  private:
    WorldManager() {}
    static b2World* world;
};
#endif // WORLD_MANAGER_H
