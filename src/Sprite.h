#ifndef SPRITE_H
#define SPRITE_H
#include "TextureRegion.h"
typedef TextureRegion Sprite;
Sprite makeSprite(int textureID, const SDL_Rect& rect);
Sprite makeSprite(int textureID);
#endif //SPRITE_H
