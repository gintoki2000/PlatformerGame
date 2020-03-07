#ifndef LEVEL_H
#define LEVEL_H
#include "GameObject.h"
#include "NTRect.h"
#include "Slime.h"
#include "WorldRenderer.h"
#include "box2d/box2d.h"
#include <string>
class Player;
class ObjectLayer;
class Monster;
class Level : public b2ContactListener
{
  public:
    static Level* create(const std::string& filename);

    ~Level();

    // stuffs methods
    void update(float dt);

    void draw(SDL_Renderer* renderer);

    // setters && getters
    b2World* getWorld() const { return m_world; }

    const NTRect& getViewPort() const { return m_viewPort; }

    Player* getPlayer() const { return m_player; }

	void addMonster(Monster* monster);

	void removeMonster(Monster* monster);

  private:
    Level();

    bool initialize(const std::string& filename);

    // Box2D callback function
    void BeginContact(b2Contact* c) override;
    void EndContact(b2Contact* c) override;

	void handleCollision(Player* player, GameObject* gameObject);

    void createGround();

    void createWall();

    void createBox(float x, float y, float width, float height);

    b2World* m_world;

    NTRect m_viewPort;

    Player* m_player;

    WorldRenderer m_worldRenderer;

    // ObjectPool
    Slime::Pool* m_slimePool;

    ObjectLayer* m_monsters;

	Monster* m_monstersToBeAdd[10];
	Monster* m_monstersToBeRemove[10];	
	int m_numMonstersToBeAdd;
	int m_numMonstersToBeRemove;
};
#endif // LEVEL_H
