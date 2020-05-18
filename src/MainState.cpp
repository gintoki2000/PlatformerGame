#include "MainState.h"
#include "Game.h"
#include "Level.h"
#include "SDL_render.h"

MainState::MainState()
{
	m_level = nullptr;
}

MainState::~MainState()
{
	delete m_level;
}

MainState* MainState::Create()
{
	MainState* ret = new MainState;
	if (ret->Init())
	{
		return ret;
	}	
	delete ret;
	return nullptr;
}

bool MainState::Init()
{
    const char* file = "asserts/levels/deep-forest.tmx";
    if ((m_level = Level::LoadFromFile(file)) == nullptr)
	{
        SDL_Log("Failed to load level: %s", file);
		return false;
	}
	m_level->Start();
	return true;
}

void MainState::Render(float deltaTime)
{
    m_level->Tick(deltaTime);
	SDL_SetRenderDrawColor(GAME->GetRenderer(), 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(GAME->GetRenderer());
    m_level->Paint();
}

void MainState::Show()
{
	m_level->SetIsPaused(false);
}

void MainState::Hidden()
{
	m_level->SetIsPaused(true);	
}

void MainState::Pause()
{
	m_level->SetIsPaused(true);
}

void MainState::Resume()
{
	m_level->SetIsPaused(false);
}
