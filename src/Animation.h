#ifndef ANIMATION_H
#define ANIMATION_H
#include <algorithm>
#include <vector>
enum AnimationType
{
    ANIMATION_TYPE_NORMAL,
    ANIMATION_TYPE_LOOP
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

    std::size_t getCurrentIndex(float timeState)
    {
        std::size_t index = static_cast<std::size_t>(timeState / m_duration);

        switch (m_type)
        {
        case ANIMATION_TYPE_NORMAL:
            index = std::max(index, m_frames.size() - 1);
            break;
        case ANIMATION_TYPE_LOOP: index = index % m_frames.size(); break;
        }
		return index;
    }

    const T& getCurrentFrame(float timeState)
    {
        return m_frames[getCurrentIndex(timeState)];
    }

    // setters and getters
    AnimationType getType() const { return m_type; }

    void setType(AnimationType type) { m_type = type; }

    const std::vector<T> getFrames() const { return m_frames; }

    void setFrames(const std::vector<T>& frames) { m_frames = frames; }

    float getDuration() const { return m_duration; }

    void setDuration(float duration) { m_duration = duration; }

  private:
    std::vector<T> m_frames;
    float m_duration;
    AnimationType m_type;
    float m_lastTimeState;
};
#endif // ANIMATION_H
