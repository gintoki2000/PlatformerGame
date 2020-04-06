#include "Animator.h"
#include "SDL_render.h"

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
	setRotation(0.0);
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
    AnimatorState& state = getCurrentState();
    if (!state.isPaused)
    {
        state.elapsedTime += deltaTime;
    }
}

void Animator::paint(SDL_Renderer* renderer)
{
    AnimatorState& state     = getCurrentState();
    Animation*     animation = m_animations[state.index];
    const Sprite*  sprite    = animation->getCurrentSprite(state.elapsedTime);
    m_dstrect.x              = m_position.x - m_origin.x;
    m_dstrect.y              = m_position.y - m_origin.y;
    m_dstrect.w              = sprite->getWidth();
    m_dstrect.h              = sprite->getHeight();
    sprite->draw(renderer, &m_dstrect, m_rotation, &m_origin, m_flip);
}

Animation* Animator::getCurrentAnimation() const
{

    return m_animations[getCurrentState().index];
}

bool Animator::isCurrentAnimationFinshed()
{
    const AnimatorState& state = getCurrentState();
    return m_animations[state.index]->isFinished(state.elapsedTime);
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

int Animator::getAnimationIndex() const { return getCurrentState().index; }

float Animator::getElapsedTime() const { return getCurrentState().elapsedTime; }

void Animator::setElapsedTime(float elapsedTime)
{
    getCurrentState().elapsedTime = elapsedTime;
}

bool Animator::isPaused() const { return getCurrentState().isPaused; }

void Animator::setPaused(bool paused) { getCurrentState().isPaused = paused; }
