#include "Game.h"
#include "SDL_timer.h"

int main()
{
        Game game;
		const int TICKS_PER_FRAME  = 16;
		Uint32 usedTicks;
		Uint32 startTicks;
        if (game.init())
        {
            while(game.isRunning())
            {
				startTicks = SDL_GetTicks();
                game.tick(1.f / 60.f);
                game.paint();
				usedTicks = SDL_GetTicks() - startTicks;
				if(usedTicks < TICKS_PER_FRAME)
						SDL_Delay(TICKS_PER_FRAME - usedTicks);
            }
        }
		return 0;
}
