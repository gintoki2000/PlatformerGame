#ifndef DESTROYABLE_H
#define DESTROYABLE_H


class IDestroyable
{
public:
    IDestroyable();
    virtual ~IDestroyable();

    virtual void takeDamge(int damage) = 0;
    virtual void getHitPoints() = 0;
    virtual void getMaxHitPoints() = 0;
    virtual void isDead() = 0;
};

#endif // DESTROYABLE_H
