#include "Game.h"
#include "AssertManager.h"
#include "BoarWarrior.h"
#include "Constances.h"
#include "GameState.h"
#include "Input.h"
#include "MainState.h"
#include "ObjectFactory.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_log.h"
#include "SDL_mixer.h"
#include "SDL_render.h"
#include "SDL_ttf.h"
#include "SDL_video.h"
#include "StateManager.h"
#include "TitleState.h"
#include "Utils.h"
#include "WorldManager.h"

Game* Game::instance = nullptr;

Game::Game()
{
    SDL_assert(instance == nullptr);
    instance    = this;
    m_isRunning = false;

    m_renderer   = nullptr;
    m_window     = nullptr;
    m_soundMGR   = nullptr;
    m_textureMGR = nullptr;
    m_stateMGR   = nullptr;
}

Game::~Game()
{
    DELETE_NULL(m_stateMGR);
	DELETE_NULL(m_soundMGR);
	DELETE_NULL(m_textureMGR);
	WorldManager::end();
	SDL_DestroyRenderer(m_renderer);
	m_renderer = nullptr;
	SDL_DestroyWindow(m_window);
	m_window = nullptr;
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
}

bool Game::init()
{
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to init SDL: %s",
                     SDL_GetError());
        return 0;
    }

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Failed to init SDL_image: %s", SDL_GetError());
        return 0;
    }

    if ((m_window = SDL_CreateWindow(
             "My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
             Constances::GAME_WIDTH * Constances::SCALE_X,
             Constances::GAME_HEIGHT * Constances::SCALE_Y,
             SDL_WINDOW_SHOWN)) == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Failed to create window: %s", SDL_GetError());
        return false;
    }
    if ((m_renderer = SDL_CreateRenderer(
             m_window, -1,
             SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE)) == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Failed to create renderer: %s", SDL_GetError());
        return false;
    }
    SDL_RenderSetScale(m_renderer, Constances::SCALE_X, Constances::SCALE_Y);

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to open SDL_mixer!");
        return false;
    }

	if (TTF_Init() == -1)
	{
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to init SDL_ttf!");
        return false;
	}

    m_textureMGR = new TextureManager();
    m_soundMGR   = new SoundManager();
    m_stateMGR   = new StateManager();

    ObjectFactory& factory = *ObjectFactory::getInstance();
    factory.resigter("BoarWarrior", BoarWarrior::create);

    if (!m_textureMGR->load(m_renderer))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load textures!");
        return false;
    }
    if (!m_soundMGR->load())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load sounds!");
        return false;
    }
    GameState* state = TitleState::create();
    if (state == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Failed to create initial state!\n");
        return false;
    }
    m_stateMGR->pushState(state);

    m_isRunning = true;
    return true;
}

void Game::render(float deltaTime)
{
    static SDL_Event event;
    m_stateMGR->update();
    GameState* state = m_stateMGR->getState();
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT: stop(); break;
        case SDL_WINDOWEVENT:
            switch (event.window.event)
            {
            case SDL_WINDOWEVENT_HIDDEN: state->hidden(); break;
            case SDL_WINDOWEVENT_SHOWN: state->show(); break;
            case SDL_WINDOWEVENT_FOCUS_GAINED: state->resume(); break;
            case SDL_WINDOWEVENT_FOCUS_LOST: state->pause(); break;
            case SDL_WINDOWEVENT_TAKE_FOCUS: state->resume(); break;
            }
			break;
        }
    }
	Input::update();
    state->render(deltaTime);
    SDL_RenderPresent(m_renderer);
}

void Game::stop() { m_isRunning = false; }
