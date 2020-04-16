#include "Locator.h"
#include "SDL_render.h"
#include "SDL_video.h"

SDL_Renderer* Locator::renderer = nullptr;
SDL_Window*   Locator::window   = nullptr;
TextureManager* Locator::textureManager = nullptr;
b2World* Locator::world = nullptr;
b2World* Locator::getWorld()
{
    return world;
}

void Locator::setWorld(b2World *value)
{
    world = value;
}

void Locator::terminate()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    delete textureManager;
	delete world;
    renderer = nullptr;
    window   = nullptr;
    textureManager = nullptr;
}

SDL_Renderer *Locator::getRenderer() { return renderer; }

void Locator::setRenderer(SDL_Renderer *_renderer) { renderer = _renderer; }

void Locator::setWindow(SDL_Window *_window) { window = _window; }

TextureManager &Locator::getTextureManager() { return *textureManager; }

void Locator::setTextureManager(TextureManager *_textureManager)
{
    textureManager = _textureManager;
}
