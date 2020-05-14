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

MainState* MainState::create()
{
	MainState* ret = new MainState;
	if (ret->init())
	{
		return ret;
	}	
	delete ret;
	return nullptr;
}

bool MainState::init()
{
    const char* file = "asserts/levels/deep-forest.tmx";
    if ((m_level = Level::loadFromFile(file)) == nullptr)
	{
        SDL_Log("Failed to load level: %s", file);
		return false;
	}
	m_level->start();
	return true;
}

void MainState::render(float deltaTime)
{
    m_level->update(deltaTime);
	SDL_SetRenderDrawColor(GAME->renderer(), 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(GAME->renderer());
    m_level->render();
}

void MainState::show()
{
	m_level->setIsPaused(false);
}

void MainState::hidden()
{
	m_level->setIsPaused(true);	
}

void MainState::pause()
{
	m_level->setIsPaused(true);
}

void MainState::resume()
{
	m_level->setIsPaused(false);
}
