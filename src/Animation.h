#ifndef ANIMATION_H
#define ANIMATION_H
#include "SpriteSheet.h"
#include <algorithm>
#include <vector>
class Animation
{
  public:
    enum PlayMode
    {
        PLAY_MODE_NORMAL,
        PLAY_MODE_LOOP,
        PLAY_MODE_LOOP_PINGPONG
    };
    Animation(const Sprite* sprites[], int numSprites, float duration,
              int playMode = PLAY_MODE_NORMAL);
    Animation(const SpriteSheet* spriteSheet, float duration,
              int playMode = PLAY_MODE_NORMAL);
    Animation(const SpriteSheet* spriteSheet, int sIndex, int numSprites,
              float duration, int playMode = PLAY_MODE_NORMAL);

    ~Animation();

    /// stuffs
    int getCurrentIndex(float elapsedTime) const;

    const Sprite* getCurrentSprite(float elapsedTime) const;

	bool isFinished(float elapsedTime) const;

    /// setter & getter

    const Sprite** getSprites() const { return m_sprites; }
    int getNumSprites() const { return m_numSprites; }
	void setSprites(const Sprite* sprites[], int numSprites);

    int getPlayMode() const { return m_playMode; }
    void setPlayMode(int mode) { m_playMode = mode; }

    float getDuration() const { return m_duration; }
    void setDuration(float duration) { m_duration = duration; }


  private:
    const Sprite** m_sprites;
    int m_numSprites;
    int m_playMode;
    float m_duration;

    mutable int m_lastIndex;
    mutable float m_lastTime;
};
#endif // ANIMATION_H
