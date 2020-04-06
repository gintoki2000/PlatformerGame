#ifndef MY_TILE_SET_H
#define MY_TILE_SET_H
#include "NTTileLayerTile.h"
#include "tmxlite/Tileset.hpp"
class Tileset
{
public:
    Tileset(const tmx::Tileset& data, SDL_Renderer* renderer);

    ~Tileset();

    NTTileLayerTile* getTile(std::size_t id);

    std::size_t getFirstGID() const { return m_firstGID; }

    std::size_t getLastGID() const { return m_firstGID + m_tileCount - 1; }

    bool hasTile(std::size_t id) const
    {
        return id >= getFirstGID() && id <= getLastGID();
    }

  private:
    std::vector<NTTileLayerTile*> m_tiles;
    std::size_t m_tileCount;
    std::size_t m_firstGID;
    SDL_Texture* m_texture;
};
#endif // MY_TILE_SET_H
