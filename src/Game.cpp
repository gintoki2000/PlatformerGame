#include "Game.h"
#include "Constances.h"
#include "GameState.h"
#include "Locator.h"
#include "MainState.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_video.h"
#include "SDL_mixer.h"
#include "StateManager.h"

Game* Game::instance = nullptr;

Game::Game() : m_isRunning(false), m_stateManager(new StateManager())
{
    instance = this;
}

Game::~Game()
{
	delete m_stateManager;
    Locator::terminate();
	Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
}

bool Game::initialize()
{
    SDL_Window*   window = nullptr;
    SDL_Renderer* renderer = nullptr;
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    {
        SDL_Log("Failed to init SDL: %s", SDL_GetError());
        return 0;
    }

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        SDL_Log("Failed to init SDL_image: %s", SDL_GetError());
        return 0;
    }

    if ((window = SDL_CreateWindow(
             "My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
             Constances::GAME_WIDTH * Constances::SCALE_X,
             Constances::GAME_HEIGHT * Constances::SCALE_Y,
             SDL_WINDOW_SHOWN)) == nullptr)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }
    if ((renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED)) ==
        nullptr)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		SDL_Log("Failed to open audio!");
		return false;
	}

    SDL_RenderSetScale(renderer, Constances::SCALE_X, Constances::SCALE_Y);

    m_isRunning = true;

    Locator::setRenderer(renderer);
    Locator::setWindow(window);
    auto state = MainState::create();
    if (state == nullptr)
    {
        SDL_Log("Failed to create initial state!\n");
        return false;
    }
    m_stateManager->pushState(state);
    return true;
}

void Game::render(float deltaTime)
{
    static SDL_Event event;
	m_stateManager->update();
    auto currentState = m_stateManager->getState();
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT: stop(); break;
        case SDL_WINDOWEVENT:
            switch (event.window.type)
            {
            case SDL_WINDOWEVENT_HIDDEN:
                currentState->hidden();
                break;
            case SDL_WINDOWEVENT_SHOWN:
                currentState->show();
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                currentState->resume();
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                currentState->pause();
                break;
            case SDL_WINDOWEVENT_TAKE_FOCUS:
                currentState->resume();
                break;
            }
        }
    }
    currentState->render(deltaTime);
    SDL_RenderPresent(Locator::getRenderer());
}

void Game::stop() { m_isRunning = false; }
