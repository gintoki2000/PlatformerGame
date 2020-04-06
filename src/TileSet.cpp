#include "TileSet.h"
#include "NTTileLayerTile.h"
#include "SDL_render.h"
#include "tmxlite/Tileset.hpp"
#include "SDL_image.h"
#include "NTRect.h"
#include "Tile.h"

Tileset::Tileset(const tmx::Tileset& data, SDL_Renderer* renderer) :
    m_tiles(data.getTileCount()), m_tileCount(data.getTileCount()),
    m_firstGID(data.getFirstGID()), m_texture(nullptr)
{

    SDL_Surface* surface = IMG_Load(data.getImagePath().c_str());
    m_texture = SDL_CreateTextureFromSurface(renderer, surface);

    std::vector<TextureRegion> staticTiles(m_tileCount);

    std::size_t index = 0;
    for (const auto& tileData : data.getTiles())
    {

        Rect rect((int)tileData.imagePosition.x,
                    (int)tileData.imagePosition.y, (int)tileData.imageSize.x,
                    (int)tileData.imageSize.y);
        staticTiles[index++] = TextureRegion(m_texture, rect);
    }

	index = 0;
	for(const auto& tileData : data.getTiles())
	{
            m_tiles[index++] = new Tile(staticTiles, tileData);
	}
	
}

Tileset::~Tileset()
{
		for(auto tile : m_tiles)
		{
				delete tile;
		}
		SDL_DestroyTexture(m_texture);
}

NTTileLayerTile* Tileset::getTile(std::size_t id)
{
		if(!hasTile(id))
				return nullptr;
		return m_tiles[id - m_firstGID];
}
