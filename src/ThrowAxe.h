#ifndef THROW_AXE_H
#define THROW_AXE_H
#include "Axe.h"
#include "DirectionalCast.h"
#include "Sprite.h"
#include "TextureManager.h"
class ThrowAxe : public DirectionalCast<Axe>
{
  public:
    static constexpr int MP_COST = 1;
    ThrowAxe() :
        DirectionalCast<Axe>(MP_COST, MakeSprite(TEX_ICONS, {16, 0, 16, 16}))
    {
    }
};
#endif // THROW_AXE_H
