#ifndef TILED_MAP_CELL_H
#define TILED_MAP_CELL_H
class TileLayerTile;
class TileLayerCell {
public:
  TileLayerCell();

  TileLayerCell(TileLayerTile *tile);

  virtual ~TileLayerCell();

  virtual TileLayerTile *GetTile();

  virtual void SetTile(TileLayerTile *tile);

protected:
  TileLayerTile *m_tile;
};
#endif // TILED_MAP_CELL_H
