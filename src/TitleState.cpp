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

TitleState* TitleState::Create()
{
    TitleState* ret = new TitleState;
    if (ret->Init())
    {
        return ret;
    }
    DELETE_NULL(ret);
    return nullptr;
}

bool TitleState::Init()
{
    const char*     text[NUM_OPTIONS] = {"Game Start"};
    const char*     ffile             = "asserts/fonts/BitPap.ttf";
    const SDL_Color white{255, 255, 255, 255};
    const SDL_Color red{255, 0, 0, 255};
    SDL_Renderer*   renderer = Game::GetInstance()->GetRenderer();
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

void TitleState::Render(float)
{
    if (Input::IsJustPressed(BUTTON_A))
    {
        Handle(m_selected);
    }
    else if (Input::IsJustPressed(BUTTON_PAD_UP))
    {
        --m_selected;
        if (m_selected < 0)
        {
            m_selected = NUM_OPTIONS - 1;
        }
    }
    else if (Input::IsJustPressed(BUTTON_PAD_DOWN))
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
            DrawToCenter(m_selectedTexts[i], startY, i, 5);
        }
        else
        {
            DrawToCenter(m_unselectedTexts[i], startY, i, 5);
        }
    }
}

void TitleState::DrawToCenter(SDL_Texture* text, int startY, int index,
                              int space)
{
    SDL_Rect dstrect;
    SDL_QueryTexture(text, nullptr, nullptr, &dstrect.w, &dstrect.h);

    dstrect.x = Constances::GAME_WIDTH / 2 - dstrect.w / 2;
    dstrect.y = startY + index * dstrect.h + space * index;

    SDL_RenderCopy(Game::GetInstance()->GetRenderer(), text, nullptr, &dstrect);
}

void TitleState::Handle(int op)
{
    switch (op)
    {
    case 0:
        Game::GetInstance()->GetStateManager().SetState(MainState::Create());
        break;
    }
}
