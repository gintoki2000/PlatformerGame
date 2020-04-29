#include "AnimatedParticle.h"
#include "Animation.h"
#include "AssertManager.h"
#include "Game.h"
#include "LayerManager.h"
#include "Utils.h"

AnimatedParticle::AnimatedParticle() : m_animation(nullptr) {}

AnimatedParticle::~AnimatedParticle() { DELETE_NULL(m_animation); }

bool AnimatedParticle::preinit(TextureManager::TextureID textureID,
                               float frameDuration, int spriteWidth,
                               int spriteHeight)
{
    SDL_Texture* texture = GAME->textureMGR().getTexture(textureID);
    m_spriteSheet.init(texture, spriteWidth, spriteHeight);
    m_animation = new Animation(&m_spriteSheet, frameDuration);
    return true;
}

void AnimatedParticle::init(const Vec2& position)
{
    m_positionX = position.x;
    m_positionY = position.y;
    m_timer     = 0.f;
}

void AnimatedParticle::tick(float deltaTime)
{
    m_timer += deltaTime;
    if (m_animation->isFinished(m_timer))
    {
        remove();
    }
}

void AnimatedParticle::paint()
{

    const Sprite& sprite = *(m_animation->getCurrentSprite(m_timer));

    const Camera& camera = getLayerManager()->getCamera();

    const SDL_Rect& viewport = camera.getViewport();

    SDL_Rect dstrect;
    dstrect.x = m_positionX - sprite.getWidth() / 2 - viewport.x;
    dstrect.y = m_positionY - sprite.getHeight() / 2 - viewport.y;
    dstrect.w = sprite.getWidth();
    dstrect.h = sprite.getHeight();

    sprite.draw(GAME->renderer(), &dstrect);
}
