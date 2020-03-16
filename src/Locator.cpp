#include "Locator.h"
#include "SDL_render.h"
#include "SDL_video.h"

SDL_Renderer* Locator::renderer = nullptr;
SDL_Window* Locator::window = nullptr;

void Locator::terminate()
{
	SDL_DestroyRenderer(Locator::renderer);
	SDL_DestroyWindow(Locator::window);
	Locator::renderer = nullptr;
	Locator::window = nullptr;
}
