#ifndef NT_TILED_MAP_STATIC_TILE_H
#define NT_TILED_MAP_STATIC_TILE_H
#include "NTTileLayerTile.h"
class NTTileLayerStaticTile : public NTTileLayerTile
{
  public:
    NTTileLayerStaticTile(const NTTextureRegion& textureRegion) :
        m_textureRegion(textureRegion)
    {
    }

    const NTTextureRegion& getTextureRegion() override;

  private:
    NTTextureRegion m_textureRegion;
};
#endif // NT_TILED_MAP_STATIC_TILE_H
