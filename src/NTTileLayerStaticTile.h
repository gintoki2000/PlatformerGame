#ifndef NT_TILED_MAP_STATIC_TILE_H
#define NT_TILED_MAP_STATIC_TILE_H
#include "NTTileLayerTile.h"
class TileLayerStaticTile : public NTTileLayerTile
{
  public:
    TileLayerStaticTile(const TextureRegion& textureRegion) :
        m_textureRegion(textureRegion)
    {
    }

    const TextureRegion& getTextureRegion() override;

  private:
    TextureRegion m_textureRegion;
};
#endif // NT_TILED_MAP_STATIC_TILE_H
