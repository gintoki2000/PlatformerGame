#include "MainState.h"
#include "Level.h"
#include "LevelParser.h"

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
	LevelParser parser;
    if ((m_level = parser.load(file)) == nullptr)
	{
        SDL_Log("Failed to load level: %s", file);
		return false;
	}
	return true;
}

void MainState::render(float deltaTime)
{
    m_level->update(deltaTime);
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
