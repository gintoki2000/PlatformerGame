#include "AnimatedParticle.h"
#include "Animation.h"
#include "Game.h"
#include "Scene.h"
#include "Utils.h"

AnimatedParticle::AnimatedParticle() : m_model(nullptr) {}

AnimatedParticle::~AnimatedParticle() { m_model = nullptr; }

void AnimatedParticle::Init(const Vec2& position)
{
    m_positionX = position.x;
    m_positionY = position.y;
    m_timer     = 0.f;
}

void AnimatedParticle::Tick(float deltaTime)
{
    m_timer += deltaTime;
    if (m_model->animation->IsFinished(m_timer))
    {
        Remove();
    }
}

void AnimatedParticle::Paint()
{

    const Sprite& sprite = *(m_model->animation->GetCurrentSprite(m_timer));

    const Camera& camera = GetScene()->GetCamera();

    const SDL_Rect& viewport = camera.GetViewport();

    SDL_Rect dstrect;
    dstrect.x = m_positionX - sprite.GetWidth() / 2 - viewport.x;
    dstrect.y = m_positionY - sprite.GetHeight() / 2 - viewport.y;
    dstrect.w = sprite.GetWidth();
    dstrect.h = sprite.GetHeight();

    sprite.Draw(GAME->GetRenderer(), &dstrect);
}
