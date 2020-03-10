#ifndef ANIMATOR_H
#define ANIMATOR_H
#include "Animation.h"
template <class T> class Animator
{
  public:
    Animator(Animation<T>* animations, int numAnimations)
    {
        initialize(animations, numAnimations);
    }
    ~Animator() { delete[] m_animations; }

    Animator& operator=(const Animator<T>& other)
    {
        delete[] m_animations;
        initialize(other.getAnimations(), other.getNumAnimations());
        return *this;
    }
    void update(float dt)
    {
        if (!m_isPaused)
        {
            m_timeState += dt;
            m_currentIndex =
                getCurrentAnimation()->getCurrentIndex(m_timeState);
            m_currentFrame = &getCurrentAnimation()
                                  ->getFrames()[(std::size_t)m_currentIndex];
        }
    }
    void play(int anim, float startTime)
    {
        m_isPaused = false;
        m_currentAnimationIndex = anim;
        m_timeState = startTime;
    }
    void pause() { m_isPaused = true; }
    void resume() { m_isPaused = false; }
    bool isPaused() const { return m_isPaused; }
    const T& getCurrentFrame() { return *m_currentFrame; }
    int getCurrentFrameIndex() { return m_currentIndex; }
    int getCurrenAnimationIndex() const { return m_currentAnimationIndex; }
    Animation<T>* getCurrentAnimation() const
    {
        return m_animations[m_currentAnimationIndex];
    }
    Animation<T>* getAnimations() const { return m_animations; }
    int getNumAnimations() const { return m_numAnimations; }

  private:
    void initialize(Animator<T>* animations, int numAnimations)
    {
        m_numAnimations = numAnimations;
        m_animations = new Animator<T>[m_numAnimations];
        for (int i = 0; i < m_numAnimations; ++i)
        {
            m_animations[i] = animations[i];
        }
        play(0, 0.f);
		update(0.f);
    }
    Animation<T>* m_animations;
    int m_numAnimations;
    float m_timeState;
    int m_currentAnimationIndex;
    bool m_isPaused;

    T* m_currentFrame;
    int m_currentIndex;
};
#endif // ANIMATOR_H
