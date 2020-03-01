#include "TileLayer.h"
#include "NTTileLayerCell.h"
#include "TileSet.h"


static NTTileLayerTile* get_tile(const std::vector<TileSet* >& tileSets, std::size_t id)
{
		for(const auto& tileSet : tileSets)
		{
				NTTileLayerTile* tile;
				if((tile = tileSet->getTile(id)) != nullptr)
						return tile;
		}
		return nullptr;
}

TileLayer::TileLayer(int width, int height, int tileWidth, int tileHeight, const tmx::TileLayer &tmxLayer, const std::vector<TileSet *>& tileSets):
    NTTileLayer (width, height, tileWidth, tileHeight)
{
    const std::vector<tmx::TileLayer::Tile>& tmxTiles = tmxLayer.getTiles();
    for(int y = 0; y < height; ++y)
    {
        for(int x = 0; x < width; ++x)
        {
            const tmx::TileLayer::Tile& tmxTile = tmxTiles[x + y * width];
            NTTileLayerTile* tile = get_tile(tileSets, tmxTile.ID);
            NTTileLayerCell* cell = new NTTileLayerCell(tile);
            setCellAt(x, y, cell);
        }
    }
    setVisible(tmxLayer.getVisible());
}
