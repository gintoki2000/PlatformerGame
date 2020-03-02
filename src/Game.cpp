#include "Game.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_render.h"

Game* Game::instance = nullptr;

Game::Game() : m_isRunning(false), m_renderer(nullptr), m_window(nullptr) { instance = this; }

Game::~Game()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    IMG_Quit();
    SDL_Quit();
}

bool Game::init() {
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    SDL_Log("Failed to init SDL: %s", SDL_GetError());
    return 0;
  }

  if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
    SDL_Log("Failed to init SDL_image: %s", SDL_GetError());
    return 0;
  }

  m_window =
      SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  if (m_window == nullptr) {
    SDL_Log("Failed to create window: %s", SDL_GetError());
    return false;
  }
  m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
  if (m_renderer == nullptr) {
    SDL_Log("Failed to create renderer: %s", SDL_GetError());
    return false;
  }

  SDL_RenderSetScale(m_renderer, SCALE_X, SCALE_Y);


  m_isRunning = true;
  return true;
}

void Game::tick(float dt)
{
    static SDL_Event event;
    while(SDL_PollEvent(&event))
    {
          if(event.type == SDL_QUIT)
            m_isRunning = false;
    }
}

void Game::paint()
{
	SDL_RenderPresent(m_renderer);
}
