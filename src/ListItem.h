#ifndef LIST_ITEM_H
#define LIST_ITEM_H
#include "SpriteSheet.h"
struct IListItem
{
    virtual ~IListItem() {}
    virtual const Sprite* getIcon() const = 0;
};
#endif // LIST_ITEM_H
