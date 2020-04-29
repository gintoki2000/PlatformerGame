#include "MyTileLayer.h"
#include "Cell.h"

MyTileLayer* MyTileLayer::create(const tmx::Map&       map,
                                 const tmx::TileLayer& data, Tilesets* tilesets)
{
    MyTileLayer* ret = new MyTileLayer;
    if (ret->init(map, data, tilesets))
    {
        return ret;
    }
    delete ret;
    return nullptr;
}

bool MyTileLayer::init(const tmx::Map& map, const tmx::TileLayer& data,
                       Tilesets* tilesets)
{
    if (!TileLayer::init(map.getTileCount().x, map.getTileCount().y,
                         map.getTileSize().x, map.getTileSize().y))
    {
        return false;
    }
    const auto& dataOfTiles = data.getTiles();
    for (int y = 0; y < getHeight(); ++y)
    {
        for (int x = 0; x < getWidth(); ++x)
        {
            const auto& tileData =
                dataOfTiles[static_cast<unsigned>(y * getWidth() + x)];
            Tile* tile = tilesets->getTile(tileData.ID);
            if (tile != nullptr)
            {
                Vec2 center(x * getTileWidth(), y * getTileHeight());
                setCellAt(x, y, Cell::create(tile, center));
            }
        }
    }
    setName(data.getName());

    return true;
}
