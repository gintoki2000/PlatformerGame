#include "TileLayerCell.h"
#include "TileLayerTile.h"
TileLayerCell::TileLayerCell() : TileLayerCell(nullptr) {}

TileLayerCell::TileLayerCell(TileLayerTile* tile) : m_tile(tile) {}

TileLayerCell::~TileLayerCell() {}

TileLayerTile* TileLayerCell::getTile() { return m_tile; }

void TileLayerCell::setTile(TileLayerTile* tile) { m_tile = tile; }
