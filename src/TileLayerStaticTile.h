#ifndef TILED_MAP_STATIC_TILE_H
#define TILED_MAP_STATIC_TILE_H
#include "TileLayerTile.h"
class TileLayerStaticTile : public TileLayerTile
{
  public:
    TileLayerStaticTile(const TextureRegion& textureRegion) :
        m_textureRegion(textureRegion)
    {
    }

    const TextureRegion& GetTextureRegion() override;

  private:
    TextureRegion m_textureRegion;
};
#endif // TILED_MAP_STATIC_TILE_H
