#ifndef CONTANCES_H
#define CONTANCES_H
#include "box2d/box2d.h"
struct Constances
{
    static constexpr float PPM = 16.f;
    static const b2Vec2    GRAVITY;
    static constexpr int   GAME_WIDTH  = 480;
    static constexpr int   GAME_HEIGHT = 320;
    static constexpr int   SCALE_X     = 3;
    static constexpr int   SCALE_Y     = 3;
    static constexpr int   TILE_SIZE   = 16;
    static constexpr float RAD_TO_DEG  = 57.2957795f;
};
#endif // CONTANCES_H
