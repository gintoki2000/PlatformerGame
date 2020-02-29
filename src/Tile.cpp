#include "Tile.h"
#include "NTTiledMapAnimatedTile.h"
#include "NTTiledMapStaticTile.h"

Tile::Tile(
    const std::vector<NTTextureRegion>& staticTiles,
    const tmx::Tileset::Tile& tileData)
{
	if(tileData.animation.frames.size() == 0)
	{
			m_tile = new NTTiledMapStaticTile(staticTiles[tileData.ID]);
	}
	else 
	{
			std::size_t size = tileData.animation.frames.size();
			std::vector<NTTextureRegion> frames(size);
			std::vector<unsigned int> intervals;
			std::size_t index = 0;
			for(const auto& frame : tileData.animation.frames)
			{
					frames[index] = staticTiles[frame.tileID];
					intervals[index] = frame.duration;
					++index;
			}
			m_tile = new NTTiledMapAnimatedTile(frames, intervals);
	}
}

Tile::~Tile()
{
		delete m_tile;
}
