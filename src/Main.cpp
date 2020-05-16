#include "Game.h"
#include "SDL_log.h"
#include "SDL_timer.h"
int main()
{
    Game        game;
    const int   TICKS_PER_FRAME = 15;
    const float TIME_STEP       = 1.f / 60.f;
    Uint32      usedTicks;
    Uint32      startTicks;
    if (game.Init())
    {
        while (game.IsRunning())
        {
            startTicks = SDL_GetTicks();
            game.Render(TIME_STEP);
            usedTicks = SDL_GetTicks() - startTicks;
            if (usedTicks < TICKS_PER_FRAME)
            {
                SDL_Delay(TICKS_PER_FRAME - usedTicks);
            }
        }
    }
    return 0;
}
