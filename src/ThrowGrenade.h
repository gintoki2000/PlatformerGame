#ifndef THROW_GRENNADE_H
#define THROW_GRENNADE_H
#include "DirectionalCast.h"
#include "Grenade.h"
#include "Sprite.h"
#include "TextureManager.h"
class ThrowGrenade : public DirectionalCast<Grenade>
{
  public:
    ThrowGrenade() :
        DirectionalCast<Grenade>(1, MakeSprite(TEX_ICONS, {16, 16, 16, 16}))
    {
    }
};
#endif // THROW_GRENNADE_H
