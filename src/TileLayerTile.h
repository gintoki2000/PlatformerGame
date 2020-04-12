#ifndef TILED_MAP_TILE_H
#define TILED_MAP_TILE_H
#include "TextureRegion.h"
class TileLayerTile {
public:
  TileLayerTile(){}
  virtual ~TileLayerTile() {}

  virtual const TextureRegion &getTextureRegion() = 0;
};
#endif // TILED_MAP_TILE_H
