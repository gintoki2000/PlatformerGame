#ifndef TMX_TILED_MAP_TILE_H
#define TMX_TILED_MAP_TILE_H
#include "TileLayerTile.h"
#include "box2d/box2d.h"
#include "tmxlite/Tileset.hpp"
#include <vector>
class Tile : public TileLayerTile
{
  public:
    Tile(const std::vector<TextureRegion>& staticTiles,
         const tmx::Tileset::Tile&         tmxTile);

    ~Tile() override;

    const TextureRegion& getTextureRegion() override
    {
        return m_tile->getTextureRegion();
    }

    int       getNumShapes() const { return m_numShapes; }
    b2Shape** getShapes() const { return m_shapes; }

  private:
    TileLayerTile* m_tile;
    b2Shape**        m_shapes;
    int              m_numShapes;
};
#endif // TMX_TILED_MAP_TILE_H
