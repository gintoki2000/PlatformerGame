#include "Game.h"
#include "Constances.h"
#include "GameState.h"
#include "Locator.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_video.h"
Game* Game::instance = nullptr;

Game::Game() : m_isRunning(false), m_state(nullptr) { instance = this; }

Game::~Game()
{
    Locator::terminate();
    IMG_Quit();
    SDL_Quit();
}

bool Game::initialize()
{
    SDL_Window* window = nullptr;
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

    SDL_RenderSetScale(renderer, Constances::SCALE_X, Constances::SCALE_Y);

    m_isRunning = true;

    Locator::setRenderer(renderer);
    Locator::setWindow(window);
    return true;
}

void Game::render(float)
{
    static SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT: stop(); break;
        case SDL_WINDOWEVENT:
            switch (event.window.type)
            {
            case SDL_WINDOWEVENT_HIDDEN: m_state->hidden(); break;
            case SDL_WINDOWEVENT_SHOWN: m_state->show(); break;
            case SDL_WINDOWEVENT_FOCUS_GAINED: m_state->resume(); break;
            case SDL_WINDOWEVENT_FOCUS_LOST: m_state->pause(); break;
            case SDL_WINDOWEVENT_TAKE_FOCUS: m_state->resume(); break;
            }
        }
    }
    SDL_RenderPresent(Locator::getRenderer());
}

void Game::stop() { m_isRunning = false; }
