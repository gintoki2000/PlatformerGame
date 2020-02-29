#ifndef MAP_H
#define MAP_H
#include "NTRect.h"
#include "SDL.h"
#include <string>
class TileLayer;
class Map
{
public:
    Map();

    ~Map();

    bool load(const std::string& filename);

    void tick(float dt);

    void paint(SDL_Renderer* renderer);

private:
    TileLayer* m_frontLayer;
    TileLayer* m_mainLayer;
    TileLayer* m_backgroundLayer;

	NTRect m_viewPort;
};

#endif // MAP_H
