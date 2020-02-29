#include "Game.h"

int main()
{
        Game game;

        if (game.init())
        {
            while(game.isRunning())
            {
                game.tick(1.f / 60.f);
                game.paint();
				SDL_Delay(100);
            }
        }

		return 0;
}
