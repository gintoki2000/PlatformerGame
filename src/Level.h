#ifndef LEVEL_H
#define LEVEL_H
#include "NTRect.h"
#include "WorldRenderer.h"
#include "box2d/b2_world_callbacks.h"
#include "box2d/box2d.h"
#include <string>
class Player;
class Level : public b2ContactListener 
{
  public:
    static Level* create(const std::string& filename);

    ~Level();

	//stuffs methods
	void update(float dt);

	void draw(SDL_Renderer* renderer);

	//setters && getters
    b2World* getWorld() const { return m_world; }

	const NTRect& getViewPort() const { return m_viewPort; }

	void BeginContact(b2Contact* c) override;

	void EndContact(b2Contact* c) override;

  private:
	Level();

	bool initialize(const std::string& filename);

	void createGround();

	void createWall();

	void createBox(float x, float y,float width, float  height);

    b2World* m_world;

	NTRect m_viewPort;

	Player* m_player;

	WorldRenderer m_worldRenderer;
};
#endif // LEVEL_H
