#include "Locator.h"
#include "Audio.h"
#include "SDL_render.h"
#include "SDL_video.h"

SDL_Renderer* Locator::renderer = nullptr;
SDL_Window*   Locator::window   = nullptr;
Audio*        Locator::audio    = nullptr;

void Locator::terminate()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    delete audio;
    renderer = nullptr;
    window   = nullptr;
    audio    = nullptr;
}
