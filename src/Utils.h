#ifndef UTILS_H
#define UTILS_H
#include "Rect.h"
#include "box2d/box2d.h"
class ICollisionHandler;
enum Tag
{
    TAG_UNDEFIED,
    TAG_BLOCK,
    TAG_PLAYER,
    TAG_MONSTER
};
struct Identifier
{
    Identifier() : tag(TAG_UNDEFIED), object(nullptr) {}
    Identifier(Tag _tag, ICollisionHandler* _object) :
        tag(_tag), object(_object)
    {
    }
    Tag                 tag;
    ICollisionHandler* object;
};

bool boxCast(const Rect& box, uint16 maskBits);
#endif // UTILS_H
