#ifndef DESTROYABLE_H
#define DESTROYABLE_H


class IDestroyable
{
public:
    IDestroyable();
    virtual ~IDestroyable();

    virtual void takeDamge(int damage) = 0;
    virtual int getHitPoints() = 0;
    virtual int getMaxHitPoints() = 0;
    virtual bool isDead() = 0;
};

#endif // DESTROYABLE_H
