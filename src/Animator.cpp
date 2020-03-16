#include "Animator.h"

Animator::Animator(Animation* animations[], int numAnimations)
{
    m_numAnimations = numAnimations;
    m_animations = new Animation*[(unsigned long)m_numAnimations];
    for (int i = 0; i < m_numAnimations; ++i)
    {
        m_animations[i] = animations[i];
    }
	play(0, 0.f);
}

void Animator::play(int index, float initialTime)
{
    m_isPaused = false;
    m_animationIndex = index;
    m_elapsedTime = initialTime;
}

void Animator::update(float deltaTime)
{
    if (!m_isPaused)
    {
        m_elapsedTime += deltaTime;
        m_currentSprite =
            m_animations[m_animationIndex]->getCurrentSprite(m_elapsedTime);
    }
}

void Animator::render(SDL_Renderer* renderer, int x, int y)
{
    m_currentSprite->draw(renderer, x, y);
}
