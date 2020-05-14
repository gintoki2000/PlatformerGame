#ifndef THROW_AXE_H
#define THROW_AXE_H
#include "Axe.h"
#include "DirectionalCast.h"
class ThrowAxe : public DirectionalCast<Axe>
{
  public:
    ThrowAxe();
    static constexpr int MP_COST = 1;
};
#endif // THROW_AXE_H
