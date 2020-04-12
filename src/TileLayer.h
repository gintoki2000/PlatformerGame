#ifndef TILE_LAYER_H
#define TILE_LAYER_H
#include "Layer.h"
#include "Rect.h"
#include "SDL2/SDL.h"
class TileLayerCell;
class TileLayer : public Layer
{

  public:
    TileLayer(int width, int height, int tileWidth, int tileHeight);

    virtual ~TileLayer();

    /// setters && getters
    int   getWidth() const { return m_width; }
    int   getHeight() const { return m_height; }
    int   getTileWidth() const { return m_tileWidth; }
    int   getTileHeight() const { return m_tileHeight; }
    float getPositionX() const { return m_positionX; }
    void  setPositionX(float positionX) { m_positionX = positionX; }
    float getPositionY() const { return m_positionY; }
    void  setPositionY(float positionY) { m_positionY = positionY; }

    /// stuffs
    virtual void     render(SDL_Renderer* renderer,
                            const Rect& viewport) override;
    TileLayerCell* getCellAt(int x, int y);
    void             setCellAt(int x, int y, TileLayerCell* cell);

  private:
    int   m_width;
    int   m_height;
    int   m_tileWidth;
    int   m_tileHeight;
    float m_positionX;
    float m_positionY;

    TileLayerCell** m_cells;
};
#endif // TILED_MAP_H
