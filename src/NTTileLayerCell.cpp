#include "NTTileLayerCell.h"
#include "NTTileLayerTile.h"
NTTileLayerCell::NTTileLayerCell() : NTTileLayerCell(nullptr) {}

NTTileLayerCell::NTTileLayerCell(NTTileLayerTile* tile) : m_tile(tile) {}

NTTileLayerCell::~NTTileLayerCell() {}

NTTileLayerTile* NTTileLayerCell::getTile() { return m_tile; }

void NTTileLayerCell::setTile(NTTileLayerTile* tile) { m_tile = tile; }
