#include "Game.h"
#include "SDL_log.h"
#include "SDL_timer.h"
void CaptureFPS(Uint32 initialTime, Uint32 tickPerFrame)
{
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - initialTime < tickPerFrame)
    {
        SDL_Delay(tickPerFrame - (currentTime - initialTime));
    }
}
int main()
{
    Game         game;
    const Uint32 TICKS_PER_FRAME = 1000 / 60;
    const float  TIME_STEP       = 1.f / 60.f;
    Uint32       initialTime;
    if (game.Init())
    {
        while (game.IsRunning())
        {
            initialTime = SDL_GetTicks();
            game.Render(TIME_STEP);
            CaptureFPS(initialTime, TICKS_PER_FRAME);
        }
    }
	// Cong la thang dbrr
    return 0;
}
