#ifndef CONTANCES_H
#define CONTANCES_H
#include <Box2D/Common/b2Math.h>
struct Constances
{
    static constexpr float PPM = 16.f;
    static const b2Vec2 GRAVITY;
    static constexpr int GAME_WIDTH = 480;
    static constexpr int GAME_HEIGHT = 320;
    static constexpr int SCALE_X = 2;
    static constexpr int SCALE_Y = 2;
};
#endif // CONTANCES_H
