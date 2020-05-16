#ifndef ANIMATOR_H
#define ANIMATOR_H
#include "Animation.h"
#include "SDL_render.h"
#include "TextureRegion.h"
struct AnimatorState
{
    AnimatorState()
    {
        index       = 0;
        elapsedTime = 0.f;
        isPaused    = false;
    }

    AnimatorState(int _index)
    {
        index       = _index;
        elapsedTime = 0.f;
        isPaused    = false;
    }

    AnimatorState(int _index, float _initialTime)
    {
        index       = _index;
        elapsedTime = _initialTime;
        isPaused    = false;
    }

    int   index;
    float elapsedTime;
    bool  isPaused;
};
class Animator
{
  public:
    Animator(Animation* animations[], int numAnimations);

    ~Animator();

    void Play(int index, float initialTime = 0.f);
    void Tick(float deltaTime);
    void Pause();
    void Resume();
    bool IsCurrentAnimationFinshed();

    /// setter & getter
    int           GetAnimationIndex() const;
    float         GetElapsedTime() const;
    void          SetElapsedTime(float elapsedTime);
    bool          IsPaused() const;
    void          SetPaused(bool paused);
    Animation*    GetCurrentAnimation() const;
    const Sprite& GetCurrentSprite() const;

    void PushState(const AnimatorState& state);
    void PopState();
    void ChangeState(const AnimatorState& state);

  private:
    Animation**      m_animations;
    int              m_numAnimations;
    SDL_Point        m_origin;
    SDL_RendererFlip m_flip;

    static constexpr int MAX_STATES = 5;
    AnimatorState        m_states[MAX_STATES];
    int                  m_numStates;
};
#endif // ANIMATOR_H
