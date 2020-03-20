#ifndef NT_TILED_MAP_H
#define NT_TILED_MAP_H
#include "NTLayer.h"
#include "NTRect.h"
#include "SDL2/SDL.h"
class NTTileLayerCell;
class NTTileLayer : public NTLayer
{

  public:
    NTTileLayer(int width, int height, int tileWidth, int tileHeight);

    virtual ~NTTileLayer();

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
                            const NTRect& viewport) override;
    NTTileLayerCell* getCellAt(int x, int y);
    void             setCellAt(int x, int y, NTTileLayerCell* cell);

  protected:
    NTTileLayer();
    virtual bool init(int width, int height, int tileWidth, int tileHeight);

  private:
    int   m_width;
    int   m_height;
    int   m_tileWidth;
    int   m_tileHeight;
    float m_positionX;
    float m_positionY;

    NTTileLayerCell** m_cells;
};
#endif // TILED_MAP_H
