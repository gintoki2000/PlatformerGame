#ifndef NT_TILED_MAP_TILE_H
#define NT_TILED_MAP_TILE_H
#include "NTTextureRegion.h"
class NTTiledMapTile {
public:
  NTTiledMapTile(){};
  virtual ~NTTiledMapTile() {}

  virtual const NTTextureRegion &getTextureRegion() = 0;
};
#endif // NT_TILED_MAP_TILE_H
