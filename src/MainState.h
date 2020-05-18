#ifndef MAIN_STATE_H
#define MAIN_STATE_H
#include "GameState.h"
class Level;
class MainState : public GameState
{
public:
		static MainState* Create();

		~MainState() override;
		void Render(float deltaTime) override;
		void Show() override;
		void Hidden() override;
		void Pause() override;
		void Resume() override;
private:
		MainState();

		bool Init();

		Level* m_level;
};
#endif //MAIN_STATE_H
