#ifndef DESTROYABLE_H
#define DESTROYABLE_H

#include "Utils.h"
class IDestroyable
{
  public:
    IDestroyable();
    virtual ~IDestroyable();

    virtual bool TakeDamge(int damage, Direction direction) = 0;
    virtual int  GetHitPoints()                             = 0;
    virtual int  GetMaxHitPoints()                          = 0;
    virtual bool IsDead()                                   = 0;
};

#endif // DESTROYABLE_H
