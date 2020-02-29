#ifndef TILELAYER_H
#define TILELAYER_H
#include "tmxlite/TileLayer.hpp"
#include "TileSet.h"
#include "NTTiledMap.h"
class TileLayer : public NTTiledMap
{
public:
    TileLayer(int width, int height, int tileWidth, int tileHeight,
              const std::vector<tmx::TileLayer::Tile>& tiles, TileSet* tileSet);
};

#endif // TILELAYER_H
