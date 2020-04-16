#ifndef TILE_LAYER_H
#define TILE_LAYER_H
#include "PositionalLayer.h"
class TileLayerCell;
class TileLayer : public PositionalLayer
{

  public:
    TileLayer(int width, int height, int tileWidth, int tileHeight);

    virtual ~TileLayer();

    /// setters && getters
    int   getWidth() const { return m_width; }
    int   getHeight() const { return m_height; }
    int   getTileWidth() const { return m_tileWidth; }
    int   getTileHeight() const { return m_tileHeight; }

    /// stuffs
	void update(float deltaTime) override;
    void render() override;
    TileLayerCell* getCellAt(int x, int y);
    void           setCellAt(int x, int y, TileLayerCell* cell);

  private:
    int   m_width;
    int   m_height;
    int   m_tileWidth;
    int   m_tileHeight;

    TileLayerCell** m_cells;
};
#endif // TILED_MAP_H
