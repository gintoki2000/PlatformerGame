#ifndef NT_TMX_TILED_MAP_TILE_H
#define NT_TMX_TILED_MAP_TILE_H
#include "NTTiledMapTile.h"
#include "tmxlite/Tileset.hpp"
#include <vector>
class Tile : public NTTiledMapTile {
public:
  Tile(const std::vector<NTTextureRegion> &staticTiles,
                    const tmx::Tileset::Tile &tileData);

  ~Tile() override;

  const NTTextureRegion &getTextureRegion() override {
    return m_tile->getTextureRegion();
  }

private:
  NTTiledMapTile *m_tile;
};
#endif // NT_TMX_TILED_MAP_TILE_H
