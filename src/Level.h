#ifndef LEVEL_H
#define LEVEL_H
#include "NTRect.h"
#include <string>
class b2World;
class Level
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

  private:
	Level();

	bool initialize(const std::string& filename);

    b2World* m_world;

	NTRect m_viewPort;
};
#endif // LEVEL_H
