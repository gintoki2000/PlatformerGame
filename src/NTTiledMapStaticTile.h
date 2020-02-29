#ifndef NT_TILED_MAP_STATIC_TILE_H
#define NT_TILED_MAP_STATIC_TILE_H
#include "NTTiledMapTile.h"
class NTTiledMapStaticTile : public NTTiledMapTile
{
  public:
    NTTiledMapStaticTile(const NTTextureRegion& textureRegion) :
        m_textureRegion(textureRegion)
    {
    }

    const NTTextureRegion& getTextureRegion() override
    {
        return m_textureRegion;
    }

  private:
    NTTextureRegion m_textureRegion;
};
#endif // NT_TILED_MAP_STATIC_TILE_H
