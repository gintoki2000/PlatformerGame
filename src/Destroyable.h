#ifndef DESTROYABLE_H
#define DESTROYABLE_H

#include "Utils.h"
class IDestroyable
{
  public:
    IDestroyable();
    virtual ~IDestroyable();

    virtual bool takeDamge(int damage, Direction direction) = 0;
    virtual int  getHitPoints()                             = 0;
    virtual int  getMaxHitPoints()                          = 0;
    virtual bool isDead()                                   = 0;
};

#endif // DESTROYABLE_H
