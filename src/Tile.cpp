#include "Tile.h"
#include "NTTileLayerStaticTile.h"
#include "NTTileLayerAnimatedTile.h"

Tile::Tile(
    const std::vector<NTTextureRegion>& staticTiles,
    const tmx::Tileset::Tile& tmxTile)
{
    if(tmxTile.animation.frames.size() == 0)
	{
            m_tile = new NTTileLayerStaticTile(staticTiles[tmxTile.ID]);
	}
	else 
	{
            std::size_t size = tmxTile.animation.frames.size();
			std::vector<NTTextureRegion> frames(size);
            std::vector<Uint32> intervals;
			std::size_t index = 0;
            for(const auto& frame : tmxTile.animation.frames)
			{
					frames[index] = staticTiles[frame.tileID];
					intervals[index] = frame.duration;
					++index;
			}
			m_tile = new NTTileLayerAnimatedTile(frames, intervals);
	}
}

Tile::~Tile()
{
		delete m_tile;
}
