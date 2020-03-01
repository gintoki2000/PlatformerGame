#ifndef NT_TILED_MAP_H
#define NT_TILED_MAP_H
#include "NTRect.h"
#include "SDL2/SDL.h"
#include "NTLayer.h"
class NTTileLayerCell;
class NTTileLayer : public NTLayer
{

  public:
    NTTileLayer(int width, int height, int tileWidth, int tileHeight);

    virtual ~NTTileLayer();

    int getWidth() const { return m_width; }

    int getHeight() const { return m_height; }

    int getTileWidth() const { return m_tileWidth; }

    int getTileHeight() const { return m_tileHeight; }

    virtual void draw(SDL_Renderer* renderer, const NTRect& viewport) override;

    virtual void update(float dt) override;

    NTTileLayerCell* getCellAt(int x, int y);

    void setCellAt(int x, int y, NTTileLayerCell* cell);

  protected:
    NTTileLayer();
    virtual bool init(int width, int height, int tileWidth, int tileHeight);
  private:
    int m_width;
    int m_height;
    int m_tileWidth;
    int m_tileHeight;

    NTTileLayerCell** m_cells;
};
#endif // TILED_MAP_H
