#ifndef LOCATOR_H
#define LOCATOR_H
#include "AssertManager.h"
#include "SDL_render.h"
#include "SDL_video.h"
#include "box2d/box2d.h"
class Locator
{
  private:
    static SDL_Renderer*   renderer;
    static SDL_Window*     window;
    static TextureManager* textureManager;

  public:
    static void terminate();

    static SDL_Renderer*   getRenderer();
    static void            setRenderer(SDL_Renderer* _renderer);
    static SDL_Window*     getWindow() { return Locator::window; }
    static void            setWindow(SDL_Window* _window);
    static TextureManager& getTextureManager();
    static void            setTextureManager(TextureManager* _textureManager);
};
#endif // LOCATOR_H
