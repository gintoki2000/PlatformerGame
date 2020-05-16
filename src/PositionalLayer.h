#ifndef POSITIONAL_LAYER_H
#define POSITIONAL_LAYER_H
#include "Layer.h"
#include "Vec.h"
class PositionalLayer : public Layer
{
  public:
    PositionalLayer();

    const Vec2&  GetPosition() const;
    virtual void SetPosition(const Vec2& position);
    float        GetPositionX() const;
    virtual void SetPositionX(float positionX);
    float        GetPositionY() const;
    virtual void SetPositionY(float positionY);
	virtual void MoveBy(const Vec2& v);

  private:
    Vec2 m_position;
};
#endif // POSITIONAL_LAYER_H
