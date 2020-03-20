#ifndef MAIN_STATE_H
#define MAIN_STATE_H
#include "GameState.h"
class Level;
class MainState : public GameState
{
public:
		static MainState* create();

		~MainState() override;
		void render(float deltaTime) override;
		void show() override;
		void hidden() override;
		void pause() override;
		void resume() override;
private:
		MainState();

		bool init();

		Level* m_level;
};
#endif //MAIN_STATE_H
