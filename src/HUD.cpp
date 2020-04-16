#include "HUD.h"
#include "Level.h"
#include "Player.h"
#include "Locator.h"

HUD::HUD()
{
    SDL_Texture* potionsTexture = Locator::getTextureManager()
            .get("asserts/spritesheets/potions-sheet.png");
    m_potionsSheet = new SpriteSheet(potionsTexture, 16, 16);
    m_animatedPotions[ANIM_RED_POTION] = new Animation(m_potionsSheet, 0, 4, Animation::PLAY_MODE_LOOP);
    m_animatedPotions[ANIM_PURPLE_POTION] = new Animation(m_potionsSheet, 4, 4, Animation::PLAY_MODE_LOOP);
    m_animatedPotions[ANIM_BLUE_POTION] = new Animation(m_potionsSheet, 8, 4, Animation::PLAY_MODE_LOOP);

    m_animationTimeState = 0.f;
}

void HUD::tick(float deltaTime)
{
    m_animationTimeState += deltaTime;
}

void HUD::paint()
{
    /*
    SDL_Renderer* renderer = Locator::getRenderer();
    const Sprite* sprite = m_animatedPotions[ANIM_RED_POTION]->getCurrentSprite(m_animationTimeState);
    SDL_Rect redPotionDst;
    redPotionDst.x = 1;
    redPotionDst.y = 1;
    redPotionDst.w = 16;
    redPotionDst.h = 16;
    sprite->draw(renderer, &redPotionDst, -30.0, nullptr, SDL_FLIP_NONE);
    float playerHitPoints = m_level->getPlayer()->getHitPoints();
    float playerMaxHitPoint = m_level->getPlayer()->getMaxHitPoints();
    float pecent = playerHitPoints / playerMaxHitPoint;
    const int HIT_POINT_BAR_LENGTH = 50;

    SDL_Rect hitPointsBar;
    hitPointsBar.x = 18;
    hitPointsBar.y = 6;
    hitPointsBar.h = 5;
    hitPointsBar.w = HIT_POINT_BAR_LENGTH * pecent;

    SDL_Rect border;
    border.x = 17;
    border.y = 5;
    border.h = 7;
    border.w = HIT_POINT_BAR_LENGTH + 2;

    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderDrawRect(renderer, &border);
    SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
    SDL_RenderFillRect(renderer, &hitPointsBar);
    */
}
