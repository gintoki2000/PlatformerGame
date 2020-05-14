#include "TextureManager.h"
#include "SDL_image.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"

SDL_Texture** TextureManager::textures = nullptr;

bool TextureManager::load(SDL_Renderer* renderer)
{
    struct Pair
    {
        int         id;
        const char* file;
    };

    unload();

    textures = (SDL_Texture**)SDL_calloc(NUM_TEXTURES, sizeof(SDL_Texture*));

    Pair pairs[] = {
        {TEX_PLAYER, "asserts/spritesheets/adventurer.png"},
        {TEX_BOAR_WARRIOR, "asserts/spritesheets/boar-warrior.png"},
        {TEX_FIRE_EXPLOSION, "asserts/spritesheets/fire-explosion.png"},
        {TEX_FIREBALL, "asserts/spritesheets/fire-ball.png"},
        {TEX_FIRE_BUST, "asserts/spritesheets/ffire-bust.png"},
        {TEX_THROWING_AXE, "asserts/spritesheets/throwing-axe.png"},
        {TEX_GRENADE, "asserts/spritesheets/grenade.png"},
        {TEX_STATUS_BAR, "asserts/spritesheets/status-bar-2.png"},
        {TEX_BLOOD_STAIN, "asserts/spritesheets/blood-stain.png"},
        {TEX_ICONS, "asserts/spritesheets/icons.png"},
    };
    int numTextures = sizeof(pairs) / sizeof(Pair);
    for (int i = 0; i < numTextures; ++i)
    {
        if ((textures[pairs[i].id] =
                 IMG_LoadTexture(renderer, pairs[i].file)) == nullptr)
        {
            return false;
        }
    }
    return true;
}

void TextureManager::unload()
{
    if (textures != nullptr)
    {
        for (int i = 0; i < NUM_TEXTURES; ++i)
        {
            SDL_DestroyTexture(textures[i]);
        }
        delete[] textures;
        textures = nullptr;
    }
}

SDL_Texture* TextureManager::get(int textureID) { return textures[textureID]; }
