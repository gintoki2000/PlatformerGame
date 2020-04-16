#ifndef CELL_H
#define CELL_H
#include "CollisionCallback.h"
#include "Tile.h"
#include "TileLayerCell.h"
#include "Utils.h"
#include "Vec.h"
class Cell : public TileLayerCell, public ICollisionCallback
{
  public:
    Cell();
	~Cell();

	virtual bool init(Tile* tlle, const Vec2& center);

    void onBeginContact(const ContactInfo& info) override;
    void onEndContact(const ContactInfo& info) override;
    void onPreSolve(const ContactInfo& info,
                    const b2Manifold&  oldManiflod) override;
    void onPostSolve(const ContactInfo&      info,
                     const b2ContactImpulse& impluse) override;

  private:
    Identifier m_identifier;
    b2Body*    m_body;
};
#endif // CELL_H
