#include "ThowAxe.h"
#include "Game.h"
#include "Sprite.h"
#include "SpriteSheet.h"
#include "TextureManager.h"

ThrowAxe::ThrowAxe() :
    DirectionalCast<Axe>(MP_COST, makeSprite(TEX_ICONS, {16, 0, 16, 16}))
{
}
