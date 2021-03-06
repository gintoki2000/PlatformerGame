#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H
#include "SDL_render.h"
enum TextureID
{
    TEX_PLAYER,
    TEX_BOAR_WARRIOR,
    TEX_FIRE_EXPLOSION,
    TEX_FIREBALL,
    TEX_FIRE_BUST,
    TEX_THROWING_AXE,
    TEX_GRENADE,
    TEX_STATUS_BAR,
    TEX_BLOOD_STAIN,
    TEX_ICONS,
	TEX_FOODS,
    NUM_TEXTURES
};
class TextureManager
{
  public:
    static bool         Load(SDL_Renderer* renderer);
    static void         Unload();
    static SDL_Texture* Get(int textureID);
  private:
	static SDL_Texture** textures;
};
#endif // TEXTURE_MANAGER_H
