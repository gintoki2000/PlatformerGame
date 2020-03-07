#include "Game.h"
#include "Constances.h"
#include "GameState.h"
#include "MainState.h"
#include "SDL.h"
#include "SDL_image.h"
Game* Game::instance = nullptr;

Game::Game() :
    m_isRunning(false), m_renderer(nullptr), m_window(nullptr), m_state(nullptr)
{
    instance = this;
}

Game::~Game()
{
    delete m_state;
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    IMG_Quit();
    SDL_Quit();
}

bool Game::initialize()
{
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

    if ((m_window = SDL_CreateWindow(
             "My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
             Constances::GAME_WIDTH * Constances::SCALE_X,
             Constances::GAME_HEIGHT * Constances::SCALE_Y,
             SDL_WINDOW_SHOWN)) == nullptr)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }
    if ((m_renderer = SDL_CreateRenderer(m_window, -1,
                                         SDL_RENDERER_ACCELERATED)) == nullptr)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    SDL_RenderSetScale(m_renderer, Constances::SCALE_X, Constances::SCALE_Y);

    if ((m_state = MainState::create()) == nullptr)
    {
        return false;
    }
    m_isRunning = true;
    return true;
}

void Game::tick(float dt)
{
    static SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            m_isRunning = false;
    }
    m_state->update(dt);
}

void Game::paint()
{
    SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(m_renderer);
    m_state->draw(m_renderer);
    SDL_RenderPresent(m_renderer);
}
