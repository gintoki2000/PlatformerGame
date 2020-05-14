#ifndef UTILS_H
#define UTILS_H
#include "Rect.h"
#include "SDL.h"
#include "Vec.h"
#include "box2d/box2d.h"
#define DELETE_NULL(ptr)                                                       \
    do                                                                         \
    {                                                                          \
        delete ptr;                                                            \
        ptr = nullptr;                                                         \
    } while (0)
class ICollisionHandler;
enum Direction
{
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
    DIRECTION_UP,
    DIRECTION_DOWN,
    DIRECTION_NONE
};

enum CateroryBit
{
    CATEGORY_BIT_PLAYER  = 1 << 0,
    CATEGORY_BIT_MONSTER = 1 << 1,
    CATEGORY_BIT_BLOCK   = 1 << 2,
    CATEGORY_BIT_SPELL   = 1 << 3
};
enum Tag
{
    TAG_UNDEFIED,
    TAG_BLOCK,
    TAG_PLAYER,
    TAG_MONSTER,
    TAG_SPELL
};

enum AttackType
{
    ATK_TYPE_SLASH,
    ATK_TYPE_STAB,
    ATK_TYPE_FIRE,
    ATK_TYPE_WATER,
    ATK_TYPE_ELECTRIC
};
struct Identifier
{
    Identifier() : tag(TAG_UNDEFIED), object(nullptr) {}
    Identifier(Tag _tag, ICollisionHandler* _object) :
        tag(_tag), object(_object)
    {
    }
    Tag                tag;
    ICollisionHandler* object;
};

bool      boxCast(const FloatRect& box, uint16 maskBits);
void      boxQuery(const FloatRect& box, uint16 maskBits, b2Fixture* fixtures[],
                   int& n, const int max);
int       directionToSign(Direction direction);
float     distance(const Vec2& a, const Vec2& b);
Direction relativeDirection(float x1, float x2);
int       randomRange(int a, int b);
#endif // UTILS_H