#include "Animator.h"
#include "SDL_render.h"
#include "GameObject.h"
#include "Level.h"
#include "Locator.h"
Animator::Animator(Animation* animations[], int numAnimations)
{
    m_numAnimations = numAnimations;
    m_animations    = new Animation*[(unsigned long)m_numAnimations];
    for (int i = 0; i < m_numAnimations; ++i)
    {
        m_animations[i] = animations[i];
    }
	m_numStates = 0;
	setOriginX(0);
    setOriginY(0);
    setFlip(SDL_FLIP_NONE);
	pushState(AnimatorState());
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
    changeState(AnimatorState(index, initialTime));
}

void Animator::tick(float deltaTime)
{
    AnimatorState& state = m_states[m_numStates - 1];
    if (!state.isPaused)
    {
        state.elapsedTime += deltaTime;
    }
}

void Animator::paint()
{
    const Sprite* sprite = getCurrentAnimation()->getCurrentSprite(getElapsedTime());
    Camera& camera = m_owner->getLayerManager()->getCamera();
    const SDL_Rect& viewport = camera.getViewport();
    SDL_Renderer* renderer = Locator::getRenderer();
    SDL_Rect dstrect;
    dstrect.x = m_owner->m_positionX - m_origin.x;
    dstrect.y = m_owner->m_positionY - m_origin.y;
    dstrect.w = sprite->getWidth();
    dstrect.h = sprite->getHeight();

    if (SDL_HasIntersection(&dstrect, &viewport))
    {
        dstrect.x -= viewport.x;
        dstrect.y -= viewport.y;
        sprite->draw(renderer, &dstrect, m_owner->m_rotation, &m_origin, m_flip);
    }

}

Animation* Animator::getCurrentAnimation() const
{

    return m_animations[m_states[m_numStates - 1].index];
}

bool Animator::isCurrentAnimationFinshed()
{
    return getCurrentAnimation()->isFinished(getElapsedTime());
}

void Animator::pushState(const AnimatorState& state)
{
    SDL_assert(m_numStates < MAX_STATES);
    m_states[m_numStates++] = state;
}

void Animator::popState()
{
    SDL_assert(m_numStates > 1);
    --m_numStates;
}

void Animator::changeState(const AnimatorState& state)
{
    m_states[m_numStates - 1] = state;
}

int Animator::getAnimationIndex() const { return m_states[m_numStates - 1].index; }

float Animator::getElapsedTime() const { return m_states[m_numStates - 1].elapsedTime; }

void Animator::setElapsedTime(float elapsedTime)
{
    m_states[m_numStates - 1].elapsedTime = elapsedTime;
}

bool Animator::isPaused() const { return m_states[m_numStates - 1].isPaused; }

void Animator::setPaused(bool paused) { m_states[m_numStates - 1].isPaused = paused; }
