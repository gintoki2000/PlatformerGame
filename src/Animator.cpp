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
	PushState(AnimatorState());
}
Animator::~Animator()
{
    for (int i = 0; i < m_numAnimations; ++i)
    {
        delete m_animations[i];
    }
    delete[] m_animations;
}

void Animator::Play(int index, float initialTime)
{
    ChangeState(AnimatorState(index, initialTime));
}

void Animator::Tick(float deltaTime)
{
    AnimatorState& state = m_states[m_numStates - 1];
    if (!state.isPaused)
    {
        state.elapsedTime += deltaTime;
    }
}

Animation* Animator::GetCurrentAnimation() const
{

    return m_animations[m_states[m_numStates - 1].index];
}

const Sprite &Animator::GetCurrentSprite() const
{
    return *GetCurrentAnimation()->GetCurrentSprite(GetElapsedTime());
}

bool Animator::IsCurrentAnimationFinshed()
{
    return GetCurrentAnimation()->IsFinished(GetElapsedTime());
}

void Animator::PushState(const AnimatorState& state)
{
    SDL_assert(m_numStates < MAX_STATES);
    m_states[m_numStates++] = state;
}

void Animator::PopState()
{
    SDL_assert(m_numStates > 1);
    --m_numStates;
}

void Animator::ChangeState(const AnimatorState& state)
{
    m_states[m_numStates - 1] = state;
}

int Animator::GetAnimationIndex() const { return m_states[m_numStates - 1].index; }

float Animator::GetElapsedTime() const { return m_states[m_numStates - 1].elapsedTime; }

void Animator::SetElapsedTime(float elapsedTime)
{
    m_states[m_numStates - 1].elapsedTime = elapsedTime;
}

bool Animator::IsPaused() const { return m_states[m_numStates - 1].isPaused; }

void Animator::SetPaused(bool paused) { m_states[m_numStates - 1].isPaused = paused; }
