#ifndef NT_TILED_MAP_CELL_H
#define NT_TILED_MAP_CELL_H
class NTTileLayerTile;
class TileLayerCell {
public:
  TileLayerCell();

  TileLayerCell(NTTileLayerTile *tile);

  virtual ~TileLayerCell();

  virtual NTTileLayerTile *getTile();

  virtual void setTile(NTTileLayerTile *tile);

protected:
  NTTileLayerTile *m_tile;
};
#endif // NT_TILED_MAP_CELL_H
