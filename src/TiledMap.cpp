#include "TiledMap.h"
#include "Cell.h"
#include "Level.h"
#include "Locator.h"
#include "NTTileLayer.h"
#include "NTTileLayerAnimatedTile.h"
#include "NTTileLayerCell.h"
static NTTileLayerTile* getTile(const std::vector<Tileset*>& tileSets,
                                std::size_t                  id)
{
    for (const auto& tileSet : tileSets)
    {
        NTTileLayerTile* tile;
        if ((tile = tileSet->getTile(id)) != nullptr)
            return tile;
    }
    return nullptr;
}

TiledMap::TiledMap(Level* level, const tmx::Map& levelData,
                   const tmx::TileLayer& tileLayerData)
{

    m_level = level;
    int width, height, tileWidth, tileHeight;
    width = (int)levelData.getTileCount().x;
    height = (int)levelData.getTileCount().y;
    tileWidth = (int)levelData.getTileSize().x;
    tileHeight = (int)levelData.getTileSize().y;
    m_tileLayer = new TileLayer(width, height, tileWidth, tileHeight);
    const auto& tmxTiles = tileLayerData.getTiles();
    const auto& tileSets = m_level->getTilesets();
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const auto&      tileData = tmxTiles[(std::size_t)(x + y * width)];
            NTTileLayerTile* tile = getTile(tileSets, tileData.ID);
            Cell*            cell = new Cell();
			cell->setTile(tile);
            setCellAt(x, y, cell);
        }
    }
    m_tileLayer->setVisible(tileLayerData.getVisible());
}

TiledMap::~TiledMap()
{
    delete m_tileLayer;
    m_tileLayer = nullptr;
}
void TiledMap::tick(float) { TileLayerAnimatedTile::updateBaseTime(); }

void TiledMap::paint()
{
    m_tileLayer->render(Locator::getRenderer(), m_level->getViewport());
}

Cell* TiledMap::getCellAt(int x, int y)
{
    return (Cell*)m_tileLayer->getCellAt(x, y);
}

void TiledMap::setCellAt(int x, int y, Cell* cell)
{
    m_tileLayer->setCellAt(x, y, (TileLayerCell*)cell);
}
