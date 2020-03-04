#ifndef ANIMATION_H
#define ANIMATION_H
#include <algorithm>
#include <vector>
enum AnimationType
{
    ANIMATION_TYPE_NORMAL,
    ANIMATION_TYPE_LOOP,
    ANIMATION_TYPE_PING_PONG_LOOP
};
template <class T> class Animation
{
  public:
    Animation() : m_duration(0.f), m_type(ANIMATION_TYPE_NORMAL) {}

    Animation(const std::vector<T>& frames, float duration,
              AnimationType type = ANIMATION_TYPE_NORMAL) :
        m_frames(frames),
        m_duration(duration), m_type(type), m_lastTimeState(0.f)
    {
    }

    // stuffs methods

    int getCurrentIndex(float timeState)
    {
        if (m_frames.size() == 1)
        {
            m_lastTimeState = timeState;
            m_lastFrameIndex = 0;
            return 0;
        }
        int index = static_cast<int>(timeState / m_duration);

        switch (m_type)
        {
        case ANIMATION_TYPE_NORMAL:
            index = std::min<int>(index, m_frames.size() - 1);
            break;
        case ANIMATION_TYPE_LOOP: index = index % m_frames.size(); break;
        case ANIMATION_TYPE_PING_PONG_LOOP:
            index = index % (m_frames.size() * 2 - 2);
            if (index >= m_frames.size())
            {
                index = m_frames.size() - 2 - (index - m_frames.size());
            }
            break;
        }
        m_lastFrameIndex = index;
        m_lastFrameIndex = index;
        return index;
    }

    const T& getCurrentFrame(float timeState)
    {
        return m_frames[getCurrentIndex(timeState)];
    }

    bool isComplete(float timeState)
    {
        if (m_frames.size() == 1)
            return true;

        int index = static_cast<int>(timeState / m_duration);
        switch (m_type)
        {
        case ANIMATION_TYPE_NORMAL: return index >= m_frames.size() - 1;
        case ANIMATION_TYPE_LOOP:
        case ANIMATION_TYPE_PING_PONG_LOOP: return false;
        }
    }

    // setters and getters
    AnimationType getType() const { return m_type; }

    void setType(AnimationType type) { m_type = type; }

    const std::vector<T> getFrames() const { return m_frames; }

    void setFrames(const std::vector<T>& frames) { m_frames = frames; }

    float getDuration() const { return m_duration; }

    void setDuration(float duration) { m_duration = duration; }

    float getLastTimeState() const { return m_lastTimeState; }

    int getLastFrameIndex() const { return m_lastFrameIndex; }

  private:
    std::vector<T> m_frames;
    float m_duration;
    AnimationType m_type;
    float m_lastTimeState;
    int m_lastFrameIndex;
};
#endif // ANIMATION_H
