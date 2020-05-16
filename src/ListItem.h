#ifndef LIST_ITEm_H
#define LIST_ITEm_H
#include "SpriteSheet.h"
struct IListItem
{
    virtual ~IListItem() {}
    virtual const Sprite* GetIcon() const = 0;
};
#endif // LIST_ITEm_H
