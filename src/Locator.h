#ifndef LOCATOR_H
#define LOCATOR_H
#include "SDL_render.h"
#include "SDL_video.h"
class Locator
{
  private:
    static SDL_Renderer* renderer;
    static SDL_Window* window;

  public:

	static void terminate();

    static SDL_Renderer* getRenderer() { return Locator::renderer; }
    static void setRenderer(SDL_Renderer* renderer)
    {
        Locator::renderer = renderer;
    };
    static SDL_Window* getWindow() { return Locator::window; }
    static void setWindow(SDL_Window* window) { Locator::window = window; }
};
#endif // LOCATOR_H
