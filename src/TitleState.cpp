#include "TitleState.h"
#include "Constances.h"
#include "Game.h"
#include "Input.h"
#include "MainState.h"
#include "SDL_log.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include "SDL_surface.h"
#include "SDL_ttf.h"
#include "StateManager.h"
#include "Utils.h"

TitleState::TitleState()
{
    SDL_memset(m_unselectedTexts, 0, NUM_OPTIONS * sizeof(SDL_Texture*));
    SDL_memset(m_selectedTexts, 0, NUM_OPTIONS * sizeof(SDL_Texture*));
}

TitleState::~TitleState()
{
    for (int i = 0; i < NUM_OPTIONS; ++i)
    {
        if (m_unselectedTexts[i] != nullptr)
        {
            SDL_DestroyTexture(m_unselectedTexts[i]);
            m_unselectedTexts[i] = nullptr;
        }
    }
}

TitleState* TitleState::create()
{
    TitleState* ret = new TitleState;
    if (ret->init())
    {
        return ret;
    }
    DELETE_NULL(ret);
    return nullptr;
}

bool TitleState::init()
{
    const char*     text[NUM_OPTIONS] = {"Start", "About", "Exit"};
    const char*     ffile             = "asserts/fonts/BitPap.ttf";
    const SDL_Color white{255, 255, 255, 255};
    const SDL_Color red{255, 0, 0, 255};
    SDL_Renderer*   renderer = Game::getInstance()->renderer();
    SDL_Surface*    tmp      = nullptr;
    TTF_Font*       font     = nullptr;
    if ((font = TTF_OpenFont(ffile, 16)) == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", TTF_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load font: %s",
                     ffile);
        return false;
    }
    for (int i = 0; i < NUM_OPTIONS; ++i)
    {
        tmp                  = TTF_RenderText_Solid(font, text[i], white);
        m_unselectedTexts[i] = SDL_CreateTextureFromSurface(renderer, tmp);
        SDL_FreeSurface(tmp);
    }
    for (int i = 0; i < NUM_OPTIONS; ++i)
    {
        tmp                = TTF_RenderText_Solid(font, text[i], red);
        m_selectedTexts[i] = SDL_CreateTextureFromSurface(renderer, tmp);
        SDL_FreeSurface(tmp);
    }
    TTF_CloseFont(font);
    m_selected = 0;
    return true;
}

void TitleState::render(float)
{
    if (Input::isButtonAJustPressed())
    {
        handle(m_selected);
    }
    else if (Input::isButtonUpJustPressed())
    {
        --m_selected;
        if (m_selected < 0)
        {
            m_selected = NUM_OPTIONS - 1;
        }
    }
    else if (Input::isButtonDownJustPressed())
    {
        ++m_selected;
        if (m_selected > NUM_OPTIONS - 1)
        {
            m_selected = 0;
        }
    }

    const int startY = 200;
    for (int i = 0; i < NUM_OPTIONS; ++i)
    {
        if (i == m_selected)
        {
            drawToCenter(m_selectedTexts[i], startY, i, 5);
        }
        else
        {
            drawToCenter(m_unselectedTexts[i], startY, i, 5);
        }
    }
}

void TitleState::drawToCenter(SDL_Texture* text, int startY, int index,
                              int space)
{
    SDL_Rect dstrect;
    SDL_QueryTexture(text, nullptr, nullptr, &dstrect.w, &dstrect.h);

    dstrect.x = Constances::GAME_WIDTH / 2 - dstrect.w / 2;
    dstrect.y = startY + index * dstrect.h + space * index;

    SDL_RenderCopy(Game::getInstance()->renderer(), text, nullptr, &dstrect);
}

void TitleState::handle(int op)
{
    switch (op)
    {
    case 0:
        Game::getInstance()->stateMGR().setState(MainState::create());
        break;
    case 1: SDL_Log("About"); break;
    case 2: Game::getInstance()->stop(); break;
    }
}
