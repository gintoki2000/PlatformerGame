#ifndef ASSERT_MANAGER_H
#define ASSERT_MANAGER_H
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_render.h"
#include <map>
#include <string>

class TextureManager
{
  public:
    enum TextureID
    {
        TEXTURE_PLAYER,
        TEXTURE_BOAR_WARRIOR,
		TEXTURE_FIRE_EXPLOSION,
		TEXTURE_FIREBALL,
        NUM_TEXTURES
    };
    TextureManager();
    ~TextureManager();

    bool         load(SDL_Renderer* renderer);
    SDL_Texture* getTexture(TextureID ID);

  private:
    SDL_Texture* m_textures[NUM_TEXTURES];

  private:
};
class SoundManager
{
  public:
    enum SoundID
    {
        SOUND_FIREBALL,
        SOUND_BOMB_EXPLOSION,
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
