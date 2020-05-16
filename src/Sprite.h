#ifndef SPRITE_H
#define SPRITE_H
#include "TextureRegion.h"
typedef TextureRegion Sprite;
Sprite MakeSprite(int textureID, const SDL_Rect& rect);
Sprite MakeSprite(int textureID);
#endif //SPRITE_H
