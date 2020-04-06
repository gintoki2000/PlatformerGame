#ifndef LOCATOR_H
#define LOCATOR_H
#include "Audio.h"
#include "SDL_render.h"
#include "SDL_video.h"
class Locator
{
  private:
    static SDL_Renderer* renderer;
    static SDL_Window*   window;
    static Audio*        audio;

  public:
    static void terminate();

    static SDL_Renderer* getRenderer() { return renderer; }
    static void setRenderer(SDL_Renderer* _renderer) { renderer = _renderer; };
    static SDL_Window* getWindow() { return Locator::window; }
    static void        setWindow(SDL_Window* _window) { window = _window; }
    static void        setAudio(Audio* _audio) { audio = _audio; }
	static Audio& getAudio() { return *audio; }
};
#endif // LOCATOR_H
