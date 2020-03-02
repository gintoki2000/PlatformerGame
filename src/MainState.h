#ifndef MAIN_STATE_H
#define MAIN_STATE_H
#include "GameState.h"
#include "Level.h"
class MainState : public GameState
{
public:
		static MainState* create();

		~MainState() override;

		void update(float dt) override;

		void draw(SDL_Renderer* renderer) override;
private:
		MainState();

		bool initialize();

		Level* m_level;
};
#endif //MAIN_STATE_H
