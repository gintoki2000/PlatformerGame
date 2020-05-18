#include "TileLayerCell.h"
#include "TileLayerTile.h"
TileLayerCell::TileLayerCell() : TileLayerCell(nullptr) {}

TileLayerCell::TileLayerCell(TileLayerTile* tile) : m_tile(tile) {}

TileLayerCell::~TileLayerCell() {}

TileLayerTile* TileLayerCell::GetTile() { return m_tile; }

void TileLayerCell::SetTile(TileLayerTile* tile) { m_tile = tile; }
