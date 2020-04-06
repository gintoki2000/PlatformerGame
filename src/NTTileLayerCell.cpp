#include "NTTileLayerCell.h"
#include "NTTileLayerTile.h"
TileLayerCell::TileLayerCell() : TileLayerCell(nullptr) {}

TileLayerCell::TileLayerCell(NTTileLayerTile* tile) : m_tile(tile) {}

TileLayerCell::~TileLayerCell() {}

NTTileLayerTile* TileLayerCell::getTile() { return m_tile; }

void TileLayerCell::setTile(NTTileLayerTile* tile) { m_tile = tile; }
