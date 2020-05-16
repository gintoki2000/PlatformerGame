#include "Game.h"
#include "Audio.h"
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
#include "TextureManager.h"
#include "TitleState.h"
#include "Utils.h"
#include "WorldManager.h"

Game* Game::instance = nullptr;

Game::Game()
{
    SDL_assert(instance == nullptr);
    instance    = this;
    m_isRunning = false;
    m_renderer  = nullptr;
    m_window    = nullptr;
    m_stateMGR  = nullptr;
}

Game::~Game()
{
    DELETE_NULL(m_stateMGR);
    WorldManager::Terminate();
    Audio::Terminate();
    TextureManager::Unload();
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
}

bool Game::Init()
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

    m_stateMGR = new StateManager();

    ObjectFactory& factory = *ObjectFactory::GetInstance();
    factory.Resigter("BoarWarrior", BoarWarrior::Create);

    Input::Init();
    Audio::Init();
    TextureManager::Load(m_renderer);
    Audio::VolumeSound(SOUND_STAB, MIX_MAX_VOLUME / 4);

    GameState* state = TitleState::Create();
    if (state == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Failed to create initial state!\n");
        return false;
    }
    m_stateMGR->PushState(state);

    m_isRunning = true;
    return true;
}

void Game::Render(float deltaTime)
{
    static SDL_Event event;
    m_stateMGR->Update();
    GameState* state = m_stateMGR->GetState();
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT: Stop(); break;
        case SDL_WINDOWEVENT:
            switch (event.window.event)
            {
            case SDL_WINDOWEVENT_HIDDEN: state->Hidden(); break;
            case SDL_WINDOWEVENT_SHOWN: state->Show(); break;
            case SDL_WINDOWEVENT_FOCUS_GAINED: state->Resume(); break;
            case SDL_WINDOWEVENT_FOCUS_LOST: state->Pause(); break;
            case SDL_WINDOWEVENT_TAKE_FOCUS: state->Resume(); break;
            }
            break;
        }
    }
    Input::Update();
    state->Render(deltaTime);
    SDL_RenderPresent(m_renderer);
}

void Game::Stop() { m_isRunning = false; }
