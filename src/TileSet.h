#ifndef MY_TILE_SET_H
#define MY_TILE_SET_H
#include "NTTiledMapTile.h"
#include "tmxlite/Tileset.hpp"
class TileSet
{
public:
    TileSet(const tmx::Tileset& data, SDL_Renderer* renderer);

    ~TileSet();

    NTTiledMapTile* getTile(std::size_t id);

    std::size_t getFirstGID() const { return m_firstGID; }

    std::size_t getLastGID() const { return m_firstGID + m_tileCount - 1; }

    bool hasTile(std::size_t id) const
    {
        return id >= getFirstGID() && id <= getFirstGID();
    }

  private:
    std::vector<NTTiledMapTile*> m_tiles;
    std::size_t m_tileCount;
    std::size_t m_firstGID;
    SDL_Texture* m_texture;
};
#endif // MY_TILE_SET_H
