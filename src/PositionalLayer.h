#ifndef POSITIONAL_LAYER_H
#define POSITIONAL_LAYER_H
#include "Layer.h"
#include "Vec.h"
class PositionalLayer : public Layer
{
  public:
    PositionalLayer();

    const Vec2&  getPosition() const;
    virtual void setPosition(const Vec2& position);
    float        getPositionX() const;
    virtual void setPositionX(float positionX);
    float        getPositionY() const;
    virtual void setPositionY(float positionY);
	virtual void moveBy(const Vec2& v);

  private:
    Vec2 m_position;
};
#endif // POSITIONAL_LAYER_H
