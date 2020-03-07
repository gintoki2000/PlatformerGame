#ifndef ENUMS_H
#define ENUMS_H
enum Direction
{
    DIRECTION_LEFT,
    DIRECTION_RIGHT
};

enum CateroryBit
{
    CATEGORY_BIT_PLAYER = 1 << 0,
    CATEGORY_BIT_MONSTER = 1 << 1,
    CATEGORY_BIT_BLOCK = 1 << 2
};

#endif // ENUMS_H
