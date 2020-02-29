#include "NTTiledMapTileSet.h"
#include "NTTiledMapTile.h"
NTTiledMapTileSet::NTTiledMapTileSet(){}

NTTiledMapTileSet::~NTTiledMapTileSet()
{
		for(auto& pair: m_tiles)
				delete pair.second;
}
