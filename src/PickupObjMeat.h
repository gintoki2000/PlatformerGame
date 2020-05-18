#ifndef PICKUPOBJECT_MEAT_H
#define PICKUPOBJECT_MEAT_H
#include "PickupObject.h"
class PickupObjectMeat : public PickupObject
{
  public:
    PickupObjectMeat(const Vec2& position);

  protected:
    void OnCollected() override;
};
#endif // PICKUPOBJECT_MEAT_H
