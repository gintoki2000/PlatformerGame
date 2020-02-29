#ifndef TILELAYER_H
#define TILELAYER_H
#include "tmxlite/TileLayer.hpp"
#include "TileSet.h"
#include "NTTiledMap.h"
class TileLayer : public NTTiledMap
{
public:
    TileLayer(int width, int height, int tileWidth, int tileHeight,
              const std::vector<tmx::TileLayer::Tile>& tiles, const std::vector<TileSet*>& tileSets);
};

#endif // TILELAYER_H
