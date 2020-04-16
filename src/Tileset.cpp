#include "Tileset.h"
#include "Rect.h"
#include "TileLayerTile.h"
#include "SDL_image.h"
#include "SDL_render.h"
#include "Tile.h"
#include "tmxlite/Tileset.hpp"

Tileset::Tileset(const tmx::Tileset& data, SDL_Renderer* renderer) :
    m_numTiles((int)data.getTileCount()), m_firstGID((int)data.getFirstGID()),
    m_texture(nullptr)
{

    SDL_Surface* surface = IMG_Load(data.getImagePath().c_str());
    m_texture            = SDL_CreateTextureFromSurface(renderer, surface);

    std::vector<TextureRegion> staticTiles((std::size_t)m_numTiles);

    std::size_t index = 0;
    for (const auto& tileData : data.getTiles())
    {

        Rect rect((int)tileData.imagePosition.x, (int)tileData.imagePosition.y,
                  (int)tileData.imageSize.x, (int)tileData.imageSize.y);
        staticTiles[index++] = TextureRegion(m_texture, rect);
    }

	m_tiles = new Tile*[(unsigned long)m_numTiles];
    index = 0;
    for (const auto& tileData : data.getTiles())
    {
        m_tiles[index++] = new Tile(staticTiles, tileData);
    }
}

Tileset::~Tileset()
{
    for (int i = 0; i < m_numTiles; ++i)
    {
        delete m_tiles[i];
        m_tiles[i] = nullptr;
    }
    delete[] m_tiles;
    SDL_DestroyTexture(m_texture);
}

Tile* Tileset::getTile(int ID) const
{
    if (!hasTile(ID))
        return nullptr;
    return m_tiles[ID - m_firstGID];
}
