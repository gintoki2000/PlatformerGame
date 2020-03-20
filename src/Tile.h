#ifndef NT_TMX_TILED_MAP_TILE_H
#define NT_TMX_TILED_MAP_TILE_H
#include "NTTileLayerTile.h"
#include "tmxlite/Tileset.hpp"
#include <vector>
class Tile : public NTTileLayerTile
{
  public:
    Tile(const std::vector<NTTextureRegion>& staticTiles,
         const tmx::Tileset::Tile&           tmxTile);

    ~Tile() override;

    const NTTextureRegion& getTextureRegion() override
    {
        return m_tile->getTextureRegion();
    }

    bool isSolid() const { return m_isSoild; }
    void setSoild(bool soild) { m_isSoild = soild; }

  private:
    NTTileLayerTile* m_tile;
    bool             m_isSoild;
};
#endif // NT_TMX_TILED_MAP_TILE_H
