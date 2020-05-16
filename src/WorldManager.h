#ifndef WORLD_MANAGER_H
#define WORLD_MANAGER_H
class b2World;
class WorldManager
{
  public:
    static b2World* GetWorld();
    static void     ClearWorld();
    static void 	Terminate();

  private:
    WorldManager() {}
    static b2World* world;
};
#endif // WORLD_MANAGER_H
