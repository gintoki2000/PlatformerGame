#include "TileLayer.h"
#include "NTTiledMapCell.h"
#include "TileSet.h"


static NTTiledMapTile* get_tile(const std::vector<TileSet* >& tileSets, std::size_t id)
{
		for(const auto& tileSet : tileSets)
		{
				NTTiledMapTile* tile;
				if((tile = tileSet->getTile(id)) != nullptr)
						return tile;
		}
		return nullptr;
}

TileLayer::TileLayer(int width, int height, int tileWidth, int tileHeight, const std::vector<tmx::TileLayer::Tile> &tmxTiles, const std::vector<TileSet *>& tileSets):
    NTTiledMap (width, height, tileWidth, tileHeight)
{
    for(int y = 0; y < height; ++y)
    {
        for(int x = 0; x < width; ++x)
        {
            const tmx::TileLayer::Tile& tmxTile = tmxTiles[x + y * width];
            NTTiledMapTile* tile = get_tile(tileSets, tmxTile.ID);
            NTTiledMapCell* cell = new NTTiledMapCell(tile);
            setCellAt(x, y, cell);
        }
    }
}
