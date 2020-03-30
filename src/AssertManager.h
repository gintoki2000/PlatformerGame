#ifndef ASSERT_MANAGER_H
#define ASSERT_MANAGER_H
#include "SDL.h"
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

  private:
	std::map<std::string, SDL_Texture*> m_textures;
	SDL_Renderer* m_renderer;
};
#endif // ASSERT_MANAGER_H
