#include "Tile.h"
#include "Builder.h"
#include "TileLayerAnimatedTile.h"
#include "TileLayerStaticTile.h"

Tile::Tile(const std::vector<TextureRegion>& staticTiles,
           const tmx::Tileset::Tile&         tileData)
{
    if (tileData.animation.frames.size() == 0)
    {
        m_tile = new TileLayerStaticTile(staticTiles[tileData.ID]);
    }
    else
    {
        std::size_t                size = tileData.animation.frames.size();
        std::vector<TextureRegion> frames(size);
        std::vector<Uint32>        intervals(size);
        std::size_t                index = 0;
        for (const auto& frame : tileData.animation.frames)
        {
            frames[index]    = staticTiles[frame.tileID - 1];
            intervals[index] = frame.duration;
            ++index;
        }
        m_tile = new TileLayerAnimatedTile(frames, intervals);
    }
    m_numShapes = tileData.objectGroup.getObjects().size();
    m_shapes    = new b2Shape*[(unsigned long)m_numShapes];
    int i       = 0;
    for (const auto& object : tileData.objectGroup.getObjects())
    {
        m_shapes[i++] = Builder::buildShape(object);
    }
}

Tile::~Tile()
{
    delete m_tile;
    for (int i = 0; i < m_numShapes; ++i)
    {
        delete m_shapes[i];
        m_shapes[i] = nullptr;
    }
}
