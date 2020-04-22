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

Animation* Animator::getCurrentAnimation() const
{

    return m_animations[m_states[m_numStates - 1].index];
}

const Sprite &Animator::getCurrentSprite() const
{
    return *getCurrentAnimation()->getCurrentSprite(getElapsedTime());
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
