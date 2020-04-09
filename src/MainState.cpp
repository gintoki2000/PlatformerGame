#include "MainState.h"
#include "Level.h"

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
    if ((m_level = Level::create(file)) == nullptr)
	{
        SDL_Log("Failed to load level: %s", file);
		return false;
	}
	return true;
}

void MainState::render(float deltaTime)
{
	m_level->render( deltaTime);
}

void MainState::show()
{
	m_level->setPaused(false);
}

void MainState::hidden()
{
	m_level->setPaused(true);	
}

void MainState::pause()
{
	m_level->setPaused(true);
}

void MainState::resume()
{
	m_level->setPaused(false);
}
