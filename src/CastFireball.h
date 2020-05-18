#ifndef CAST_FIRE_BALL_H
#define CAST_FIRE_BALL_H
#include "DirectionalCast.h"
#include "Fireball.h"
#include "Sprite.h"
#include "TextureManager.h"
class CastFireBall : public DirectionalCast<Fireball>
{
  public:
    CastFireBall() :
        DirectionalCast<Fireball>(1, MakeSprite(TEX_ICONS, {0, 16, 16, 16}))
    {
    }
};
#endif // CAST_FIRE_BALL_H
