#include "HUD.h"
#include "AssertManager.h"
#include "Game.h"
#include "Level.h"
#include "Player.h"
#include "SDL_render.h"
#include "SpriteSheet.h"
#include "Utils.h"

HUD::HUD() {}

HUD* HUD::create()
{
    HUD* ret = new HUD;
    if (ret->init())
    {
        return ret;
    }
    DELETE_NULL(ret);
    return nullptr;
}

bool HUD::init()
{
    TextureManager& textureMGR = Game::getInstance()->textureMGR();
    SDL_Texture*    texture = textureMGR.getTexture(TextureManager::STATUS_BAR);

    m_circle   = Sprite(texture, {0, 0, 65, 61});
    m_mpBorder = Sprite(texture, {0, 89, 132, 23});
    m_hpBorder = Sprite(texture, {0, 61, 137, 28});

    m_hp.init(texture, 64, 0, 2, 12, 16, 11);
    m_mp.init(texture, 65, 22, 3, 7, 11, 6);

    return true;
}

void HUD::tick(Level&, float) {}

void HUD::paint(Level& level)
{

    SDL_Renderer* renderer;
    SDL_Rect      dstrect;
    SDL_Rect      srcrect;
    Player*       player;

    renderer = Game::getInstance()->renderer();
    player   = level.getPlayer();

    int drawPosX = 5;
    int drawPosY = 3;

    float hpPercent = static_cast<float>(player->getHitPoints()) /
                      static_cast<float>(player->getMaxHitPoints());

    float mpPercent = static_cast<float>(player->getManaPoints()) /
                      static_cast<float>(player->getMaxManaPoints());

    // draw hitPointBar
    dstrect.x = drawPosX + 57;
    dstrect.y = drawPosY + 10;
    dstrect.w = m_hpBorder.getWidth();
    dstrect.h = m_hpBorder.getHeight();
    m_hpBorder.draw(renderer, &dstrect);

    dstrect.x = drawPosX + 62 - 16;
    dstrect.y = drawPosY + 22;
    dstrect.w = 16;
    dstrect.h = 11;

    int n = hpPercent * m_hp.getNumSprites();
    for (int i = 0; i < n; ++i)
    {
        m_hp.getSprite(i).draw(renderer, &dstrect);
        dstrect.x += 6;
    }

    // draw manaBar
    dstrect.x = drawPosX + 57;
    dstrect.y = drawPosY + 26;
    dstrect.w = m_mpBorder.getWidth();
    dstrect.h = m_mpBorder.getHeight();
    m_mpBorder.draw(renderer, &dstrect);

    dstrect.x = drawPosX + 53;
    dstrect.y = drawPosY + 38;
    dstrect.w = 11;
    dstrect.h = 6;

    n = mpPercent * m_mp.getNumSprites();

    for (int i = 0; i < n; ++i)
    {
        m_mp.getSprite(i).draw(renderer, &dstrect);
        dstrect.x += 6;
    }

    // draw circle
    m_circle.draw(renderer, drawPosX, drawPosY);
}
