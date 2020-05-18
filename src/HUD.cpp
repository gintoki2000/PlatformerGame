#include "HUD.h"
#include "Adventurer.h"
#include "Game.h"
#include "Level.h"
#include "SDL_render.h"
#include "SpriteSheet.h"
#include "TextureManager.h"
#include "Utils.h"

HUD::HUD()
{

    SDL_Texture* texture = TextureManager::Get(TEX_STATUS_BAR);

    m_circle   = Sprite(texture, {0, 0, 65, 61});
    m_mpBorder = Sprite(texture, {0, 89, 132, 23});
    m_hpBorder = Sprite(texture, {0, 61, 137, 28});

    m_hp.Init(texture, 64, 0, 2, 12, 16, 11);
    m_mp.Init(texture, 65, 22, 3, 7, 11, 6);

}

void HUD::Tick(float) {}

void HUD::Paint()
{
    SDL_Renderer* renderer;
    SDL_Rect      dstrect;
    Adventurer*   adventurer;
    Level*        level = static_cast<Level*>(GetScene());

    renderer   = Game::GetInstance()->GetRenderer();
    adventurer = level->GetAdventurer();

    int drawPosX = 5;
    int drawPosY = 3;

    float hpPercent = static_cast<float>(adventurer->GetHitPoints()) /
                      static_cast<float>(adventurer->GetMaxHitPoints());

    float mpPercent = static_cast<float>(adventurer->GetManaPoints()) /
                      static_cast<float>(adventurer->GetMaxManaPoints());

    // draw hitPointBar
    dstrect.x = drawPosX + 57;
    dstrect.y = drawPosY + 10;
    dstrect.w = m_hpBorder.GetWidth();
    dstrect.h = m_hpBorder.GetHeight();
    m_hpBorder.Draw(renderer, &dstrect);

    dstrect.x = drawPosX + 62 - 16;
    dstrect.y = drawPosY + 22;
    dstrect.w = 16;
    dstrect.h = 11;

    int n = hpPercent * m_hp.GetNumSprites();
    for (int i = 0; i < n; ++i)
    {
        m_hp.GetSprite(i).Draw(renderer, &dstrect);
        dstrect.x += 6;
    }

    // draw manaBar
    dstrect.x = drawPosX + 57;
    dstrect.y = drawPosY + 26;
    dstrect.w = m_mpBorder.GetWidth();
    dstrect.h = m_mpBorder.GetHeight();
    m_mpBorder.Draw(renderer, &dstrect);

    dstrect.x = drawPosX + 53;
    dstrect.y = drawPosY + 38;
    dstrect.w = 11;
    dstrect.h = 6;

    n = mpPercent * m_mp.GetNumSprites();

    for (int i = 0; i < n; ++i)
    {
        m_mp.GetSprite(i).Draw(renderer, &dstrect);
        dstrect.x += 6;
    }

    // draw circle
    m_circle.Draw(renderer, drawPosX, drawPosY);
}
