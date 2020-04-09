#ifndef TILED_MAP_H
#define TILED_MAP_H
#include "NTTileLayer.h"
#include "NTTileLayerCell.h"
#include "Tileset.h"
#include "tmxlite/Map.hpp"
#include "tmxlite/TileLayer.hpp"
class Level;
class Cell;
class TiledMap
{
  public:
    TiledMap(Level* level, const tmx::Map& levelData,
             const tmx::TileLayer& tileLayerData);
    ~TiledMap();
    void tick(float deltaTime);
    void paint();

    Cell* getCellAt(int x, int y);
    void  setCellAt(int x, int y, Cell* cell);
    int   getWidth() const { return m_tileLayer->getWidth(); }
    int   getHeight() const { return m_tileLayer->getHeight(); }
    int   getTileWidth() const { return m_tileLayer->getTileWidth(); }
    int   getTileHeight() const { return m_tileLayer->getTileHeight(); }

  private:
    Level*     m_level;
    TileLayer* m_tileLayer;
};
#endif // TILED_MAP_H
