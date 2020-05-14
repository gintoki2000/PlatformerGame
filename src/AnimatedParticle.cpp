#include "AnimatedParticle.h"
#include "Animation.h"
#include "Game.h"
#include "Scene.h"
#include "Utils.h"

AnimatedParticle::AnimatedParticle() : m_model(nullptr) {}

AnimatedParticle::~AnimatedParticle() { m_model = nullptr; }

void AnimatedParticle::init(const Vec2& position)
{
    m_positionX = position.x;
    m_positionY = position.y;
    m_timer     = 0.f;
}

void AnimatedParticle::tick(float deltaTime)
{
    m_timer += deltaTime;
    if (m_model->animation->isFinished(m_timer))
    {
        remove();
    }
}

void AnimatedParticle::paint()
{

    const Sprite& sprite = *(m_model->animation->getCurrentSprite(m_timer));

    const Camera& camera = getScene()->getCamera();

    const SDL_Rect& viewport = camera.getViewport();

    SDL_Rect dstrect;
    dstrect.x = m_positionX - sprite.getWidth() / 2 - viewport.x;
    dstrect.y = m_positionY - sprite.getHeight() / 2 - viewport.y;
    dstrect.w = sprite.getWidth();
    dstrect.h = sprite.getHeight();

    sprite.draw(GAME->renderer(), &dstrect);
}
