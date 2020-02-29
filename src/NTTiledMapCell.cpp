#include "NTTiledMapCell.h"
#include "NTTiledMapTile.h"
NTTiledMapCell::NTTiledMapCell() : NTTiledMapCell(nullptr) {}

NTTiledMapCell::NTTiledMapCell(NTTiledMapTile *tile) : m_tile(tile) {}

NTTiledMapCell::~NTTiledMapCell()
{
}

NTTiledMapTile *NTTiledMapCell::getTile() { return m_tile; }

void NTTiledMapCell::setTile(NTTiledMapTile *tile)
{
		m_tile = tile;
}


