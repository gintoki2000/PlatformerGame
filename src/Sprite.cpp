#include "Sprite.h"
#include "SDL_render.h"
#include "TextureManager.h"

Sprite makeSprite(int textureID, const SDL_Rect& rect)
{
    return Sprite(TextureManager::get(textureID), rect);
}

Sprite makeSprite(int textureID)
{
    return Sprite(TextureManager::get(textureID));
}
