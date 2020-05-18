#include "Sprite.h"
#include "SDL_render.h"
#include "TextureManager.h"

Sprite MakeSprite(int textureID, const SDL_Rect& rect)
{
    return Sprite(TextureManager::Get(textureID), rect);
}

Sprite MakeSprite(int textureID)
{
    return Sprite(TextureManager::Get(textureID));
}
