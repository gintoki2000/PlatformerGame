#include "MyTileLayer.h"
#include "Cell.h"

MyTileLayer* MyTileLayer::Create(const tmx::Map&       map,
                                 const tmx::TileLayer& data, Tilesets* tilesets)
{
    MyTileLayer* ret = new MyTileLayer;
    if (ret->Init(map, data, tilesets))
    {
        return ret;
    }
    delete ret;
    return nullptr;
}

bool MyTileLayer::Init(const tmx::Map& map, const tmx::TileLayer& data,
                       Tilesets* tilesets)
{
    if (!TileLayer::Init(map.getTileCount().x, map.getTileCount().y,
                         map.getTileSize().x, map.getTileSize().y))
    {
        return false;
    }
    const auto& dataOfTiles = data.getTiles();
    for (int y = 0; y < GetHeight(); ++y)
    {
        for (int x = 0; x < GetWidth(); ++x)
        {
            const auto& tileData =
                dataOfTiles[static_cast<unsigned>(y * GetWidth() + x)];
            Tile* tile = tilesets->GetTile(tileData.ID);
            if (tile != nullptr)
            {
                Vec2 center(x * GetTileWidth(), y * GetTileHeight());
                SetCellAt(x, y, Cell::Create(tile, center));
            }
        }
    }
    SetName(data.getName());

    return true;
}
