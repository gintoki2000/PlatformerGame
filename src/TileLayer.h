#ifndef TILELAYER_H
#define TILELAYER_H
#include "tmxlite/TileLayer.hpp"
#include "TileSet.h"
#include "NTTileLayer.h"
class TileLayer : public NTTileLayer
{
public:
    TileLayer(int width, int height, int tileWidth, int tileHeight,
              const tmx::TileLayer& tmxLayer, const std::vector<TileSet*>& tileSets);
};

#endif // TILELAYER_H
