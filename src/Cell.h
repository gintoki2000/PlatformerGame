#ifndef CELL_H
#define CELL_H
#include "CollisionHandler.h"
#include "Tile.h"
#include "TileLayerCell.h"
#include "Utils.h"
#include "Vec.h"
class Cell : public TileLayerCell, public ICollisionHandler
{
  public:
    ~Cell();

    static Cell* Create(Tile* tile, const Vec2& center);

    void OnBeginContact(const ContactInfo& info) override;
    void OnEndContact(const ContactInfo& info) override;
    void OnPreSolve(const ContactInfo& info,
                    const b2Manifold&  oldManiflod) override;
    void OnPostSolve(const ContactInfo&      info,
                     const b2ContactImpulse& impluse) override;

  private:
    Cell();
    virtual bool Init(Tile* tile, const Vec2& center);
    Identifier   m_identifier;
    b2Body*      m_body;
};
#endif // CELL_H
