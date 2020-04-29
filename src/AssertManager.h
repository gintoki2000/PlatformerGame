#ifndef ASSERT_MANAGER_H
#define ASSERT_MANAGER_H
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_render.h"

class TextureManager
{
  public:
    enum TextureID
    {
        PLAYER,
        BOAR_WARRIOR,
        FIRE_EXPLOSION,
        FIREBALL,
        FIRE_BUST,
        THROWING_AXE,
        GRENADE,
        STATUS_BAR,
		BLOOD_STAIN,
        NUM_TEXTURES
    };
    TextureManager();
    ~TextureManager();

    bool         load(SDL_Renderer* renderer);
    SDL_Texture* getTexture(TextureID ID);

  private:
    SDL_Texture* m_textures[NUM_TEXTURES];
};
class SoundManager
{
  public:
    enum SoundID
    {
        FIREBALL,
        IMPACT,
        BOMB_EXPLOSION,
        NUM_SOUNDS
    };

    SoundManager();
    ~SoundManager();

    bool       load();
    Mix_Chunk* getSound(SoundID ID);

  private:
    Mix_Chunk* m_sounds[NUM_SOUNDS];
};

#endif // ASSERT_MANAGER_H
