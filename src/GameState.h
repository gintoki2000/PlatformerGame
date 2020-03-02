#ifndef GAME_STATE_H
#define GAME_STATE_H
#include "SDL_render.h"
class GameState 
{
	public:
			GameState() {}

			virtual ~GameState() {}

			virtual void update(float dt) = 0;

			virtual void draw(SDL_Renderer* renderer) = 0;
};
#endif //GAME_STATE_H
