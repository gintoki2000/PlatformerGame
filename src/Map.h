#ifndef MAP_H
#define MAP_H
#include "NTRect.h"
#include "SDL.h"
#include <string>
#include <vector>
class TileLayer;
class TileSet;
class Map
{
public:
    Map();

    ~Map();

    bool load(const std::string& filename);

    void tick(float dt);

    void paint();

private:
    TileLayer* m_frontLayer;
    TileLayer* m_mainLayer;
    TileLayer* m_backgroundLayer;

	std::vector<TileSet*> m_tileSets;

	NTRect m_viewPort;

};

#endif // MAP_H
