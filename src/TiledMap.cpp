#include "TiledMap.h"
#include "Cell.h"
#include "Constances.h"
#include "Enums.h"
#include "Level.h"
#include "Locator.h"
#include "NTTileLayer.h"
#include "NTTileLayerAnimatedTile.h"
#include "NTTileLayerCell.h"
#include "Tilesets.h"

TiledMap::TiledMap(Level* level, const tmx::Map& levelData,
                   const tmx::TileLayer& tileLayerData)
{

    m_level = level;
    int width, height, tileWidth, tileHeight;
    width                = (int)levelData.getTileCount().x;
    height               = (int)levelData.getTileCount().y;
    tileWidth            = (int)levelData.getTileSize().x;
    tileHeight           = (int)levelData.getTileSize().y;
    m_tileLayer          = new TileLayer(width, height, tileWidth, tileHeight);
    const auto& tmxTiles = tileLayerData.getTiles();
    Tilesets*   tileSets = m_level->getTilesets();
    b2BodyDef   bdef;
    bdef.type = b2_staticBody;

    b2FixtureDef fdef;
    fdef.filter.categoryBits = CATEGORY_BIT_BLOCK;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const auto& tileData = tmxTiles[(std::size_t)(x + y * width)];
            Tile*       tile     = tileSets->getTile((int)tileData.ID);
            if (tile != nullptr)
            {
                bdef.position.x    = (x * tileWidth) / Constances::PPM;
                bdef.position.y    = (y * tileHeight) / Constances::PPM;
                b2Body*   cellBody = m_level->getWorld()->CreateBody(&bdef);
                b2Shape** shapes   = tile->getShapes();
                for (int i = 0; i < tile->getNumShapes(); ++i)
                {
                    fdef.shape = shapes[i];
                    cellBody->CreateFixture(&fdef);
                }
                Cell* cell = new Cell();
                cell->setTile(tile);
                setCellAt(x, y, cell);
            }
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
