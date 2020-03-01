#ifndef NT_TILED_MAP_CELL_H
#define NT_TILED_MAP_CELL_H
class NTTileLayerTile;
class NTTileLayerCell {
public:
  NTTileLayerCell();

  NTTileLayerCell(NTTileLayerTile *tile);

  virtual ~NTTileLayerCell();

  virtual NTTileLayerTile *getTile();

  virtual void setTile(NTTileLayerTile *tile);

protected:
  NTTileLayerTile *m_tile;
};
#endif // NT_TILED_MAP_CELL_H
