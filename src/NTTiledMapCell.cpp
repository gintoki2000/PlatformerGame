#include "NTTiledMapCell.h"
#include "NTTiledMapTile.h"
NTTiledMapCell::NTTiledMapCell() : NTTiledMapCell(nullptr) {}

NTTiledMapCell::NTTiledMapCell(NTTiledMapTile *tile) : m_tile(tile) {}

NTTiledMapCell::~NTTiledMapCell()
{
		delete m_tile;
}

NTTiledMapTile *NTTiledMapCell::getTile() { return m_tile; }

void NTTiledMapCell::setTile(NTTiledMapTile *tile)
{
		if(m_tile != nullptr)
				delete m_tile;
		m_tile = tile;
}


