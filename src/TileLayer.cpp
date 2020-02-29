#include "TileLayer.h"
#include "NTTiledMapCell.h"


TileLayer::TileLayer(int width, int height, int tileWidth, int tileHeight, const std::vector<tmx::TileLayer::Tile> &tmxTiles, TileSet *tileSet):
    NTTiledMap (width, height, tileWidth, tileHeight)
{
    for(int y = 0; y < height; ++y)
    {
        for(int x = 0; x < width; ++x)
        {
            const tmx::TileLayer::Tile& tmxTile = tmxTiles[x + y * width];
            NTTiledMapTile* tile = tileSet->getTile(tmxTile.ID);
            NTTiledMapCell* cell = new NTTiledMapCell(tile);
            setCellAt(x, y, cell);
        }
    }
}
