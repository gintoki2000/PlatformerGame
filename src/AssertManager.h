#ifndef ASSERT_MANAGER_H
#define ASSERT_MANAGER_H
#include "SDL.h"
#include "SDL_mixer.h"
#include <map>
#include <string>

class TextureManager
{
  public:
    TextureManager(SDL_Renderer* renderer);
    ~TextureManager();

	bool isLoaded(const std::string& filename);
	bool load(const std::string& filename);
	SDL_Texture* get(const std::string& filename);

	void unloadAll();

  private:
	std::map<std::string, SDL_Texture*> m_textures;
	SDL_Renderer* m_renderer;
};
class SoundManager 
{
	public:
		enum SoundID 
		{
			SOUND_FIREBALL,
			NUM_SOUNDS
		};
		SoundManager();
		~SoundManager();
	private:
		Mix_Chunk* m_sounds[NUM_SOUNDS];
};
#endif // ASSERT_MANAGER_H
