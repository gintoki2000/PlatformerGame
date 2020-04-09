#ifndef MY_TILE_SET_H
#define MY_TILE_SET_H
#include "Tile.h"
#include "tmxlite/Tileset.hpp"
class Tileset
{
  public:
    Tileset(const tmx::Tileset& data, SDL_Renderer* renderer);

    ~Tileset();

    Tile* getTile(int ID);

    int getFirstGID() const { return m_firstGID; }

    int getLastGID() const { return m_firstGID + m_numTiles - 1; }

    bool hasTile(int ID) const
    {
        return ID >= getFirstGID() && ID <= getLastGID();
    }

  private:
    Tile**       m_tiles;
    int          m_numTiles;
    int          m_firstGID;
    SDL_Texture* m_texture;
};
#endif // MY_TILE_SET_H
