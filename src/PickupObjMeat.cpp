#include "PickupObjMeat.h"
#include "Adventurer.h"
#include "Level.h"
#include "PickupObject.h"
#include "Sprite.h"
#include "SpriteSheet.h"
#include "TextureManager.h"

PickupObjectMeat::PickupObjectMeat(const Vec2& position) :
    PickupObject(MakeSprite(TEX_FOODS, {16, 16, 16, 16}), position)
{
}

void PickupObjectMeat::OnCollected() {}
