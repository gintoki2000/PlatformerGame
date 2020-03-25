#include "Animator.h"

Animator::Animator(Animation* animations[], int numAnimations)
{
    m_numAnimations = numAnimations;
    m_animations    = new Animation*[(unsigned long)m_numAnimations];
    for (int i = 0; i < m_numAnimations; ++i)
    {
        m_animations[i] = animations[i];
    }
    play(0, 0.f);
	m_origin.x = 0;
	m_origin.y = 0;
}
Animator::~Animator()
{
    for (int i = 0; i < m_numAnimations; ++i)
    {
        delete m_animations[i];
    }
    delete[] m_animations;
}

void Animator::play(int index, float initialTime)
{
    m_isPaused       = false;
    m_animationIndex = index;
    m_elapsedTime    = initialTime;
    m_currentSprite =
        m_animations[m_animationIndex]->getCurrentSprite(m_elapsedTime);
}

void Animator::tick(float deltaTime)
{
    if (!m_isPaused)
    {
        m_elapsedTime += deltaTime;
        m_currentSprite =
            m_animations[m_animationIndex]->getCurrentSprite(m_elapsedTime);
    }
}

void Animator::paint(SDL_Renderer* renderer)
{
    m_dstrect.x = m_position.x - m_origin.x;
    m_dstrect.y = m_position.y - m_origin.y;
    m_dstrect.w = m_currentSprite->getWidth();
    m_dstrect.h = m_currentSprite->getHeight();
    m_currentSprite->draw(renderer, &m_dstrect, m_rotation, &m_origin, m_flip);
}
bool Animator::isCurrentAnimationFinshed()
{
    return m_animations[m_animationIndex]->isFinished(m_elapsedTime);
}
