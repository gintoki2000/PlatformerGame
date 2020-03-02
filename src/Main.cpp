#include "Game.h"
#include "SDL_timer.h"

int main()
{
    Game game;
    const int TICKS_PER_FRAME = 16;
    const float TIME_STEP = 1.f / 60.f;
    Uint32 usedTicks;
    Uint32 startTicks;
    if (game.initialize())
    {
        while (game.isRunning())
        {
            startTicks = SDL_GetTicks();
            game.tick(TIME_STEP);
            game.paint();
            usedTicks = SDL_GetTicks() - startTicks;
            if (usedTicks < TICKS_PER_FRAME)
                SDL_Delay(TICKS_PER_FRAME - usedTicks);
        }
    }
    return 0;
}
