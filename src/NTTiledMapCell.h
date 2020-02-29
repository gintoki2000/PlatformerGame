#ifndef NT_TILED_MAP_CELL_H
#define NT_TILED_MAP_CELL_H
class NTTiledMapTile;
class NTTiledMapCell {
public:
  NTTiledMapCell();

  NTTiledMapCell(NTTiledMapTile *tile);

  virtual ~NTTiledMapCell();

  virtual NTTiledMapTile *getTile();

  virtual void setTile(NTTiledMapTile *tile);

protected:
  NTTiledMapTile *m_tile;
};
#endif // NT_TILED_MAP_CELL_H
