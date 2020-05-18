#ifndef TILE_LAYER_H
#define TILE_LAYER_H
#include "PositionalLayer.h"
class TileLayerCell;
class TileLayer : public PositionalLayer
{

  public:
	
	static TileLayer* Create(int width, int height, int tileWidth, int tileHeight);

    ~TileLayer();

    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    int GetTileWidth() const { return m_tileWidth; }
    int GetTileHeight() const { return m_tileHeight; }

    void           Tick(float deltaTime) override;
    void           Paint() override;
    TileLayerCell* GetCellAt(int x, int y);
    void           SetCellAt(int x, int y, TileLayerCell* cell);

  private:
    int             m_width;
    int             m_height;
    int             m_tileWidth;
    int             m_tileHeight;
    TileLayerCell** m_cells;

  protected:
	TileLayer();
	bool Init(int width, int height, int tileWidth, int tileHeight);
};
#endif // TILED_MAP_H
