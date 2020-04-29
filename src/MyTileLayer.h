#ifndef MY_TILE_LAYER_H
#define MY_TILE_LAYER_H
#include "TileLayer.h"
#include "Tilesets.h"
#include "tmxlite/Map.hpp"
#include "tmxlite/TileLayer.hpp"
class MyTileLayer : public TileLayer
{
  public:
    static MyTileLayer* create(const tmx::Map& map, const tmx::TileLayer& data,
                               Tilesets* tilesets);

  private:
    MyTileLayer() {}
    bool init(const tmx::Map& map, const tmx::TileLayer& data,
              Tilesets* tilesets);
};
#endif // MY_TILE_LAYER_H
