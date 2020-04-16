#ifndef UTILS_H
#define UTILS_H
class ICollisionCallback;
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
    Identifier(Tag _tag, ICollisionCallback* _object) :
        tag(_tag), object(_object)
    {
    }
    Tag                 tag;
    ICollisionCallback* object;
};

#endif // UTILS_H
