#ifndef NT_TILED_MAP_H
#define NT_TILED_MAP_H
#include "NTRect.h"
#include "SDL2/SDL.h"
class NTTiledMapCell;
class NTTiledMap
{

  public:
    NTTiledMap(int width, int height, int tileWidth, int tileHeight);

    virtual ~NTTiledMap();

    int getWidth() const { return m_width; }

    int getHeight() const { return m_height; }

    int getTileWidth() const { return m_tileWidth; }

    int getTileHeight() const { return m_tileHeight; }

    virtual void draw(SDL_Renderer* renderer, const NTRect& viewport);

    NTTiledMapCell* getCellAt(int x, int y);

    void setCellAt(int x, int y, NTTiledMapCell* cell);

    int getPositionX() const { return m_positionX; }

    void setPositionX(int positionX) { m_positionX = positionX; }

    int getPositionY() const { return m_positionY; }

    void setPositionY(int positionY) { m_positionY = positionY; }

  protected:
	NTTiledMap();
    virtual bool init(int width, int height, int tileWidth, int tileHeight);
  private:
    int m_width;
    int m_height;
    int m_tileWidth;
    int m_tileHeight;

    int m_positionX;
    int m_positionY;

    NTTiledMapCell** m_cells;
};
#endif // TILED_MAP_H
