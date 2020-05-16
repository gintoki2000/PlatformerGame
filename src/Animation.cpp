#include "Animation.h"
#include "SpriteSheet.h"
#include "Math.h"
Animation::Animation(const Sprite* sprites[], int numSprites, float duration,
                     int playMode) :
    m_sprites(nullptr)
{
    SetSprites(sprites, numSprites);
    m_duration = duration;
    m_playMode = playMode;
}

Animation::Animation(const SpriteSheet* spriteSheet, float duration,
                     int playMode) :
    Animation(spriteSheet, 0, spriteSheet->GetNumSprites(), duration, playMode)
{
}

Animation::Animation(const SpriteSheet* spriteSheet, int sIndex, int numSprites,
                     float duration, int playMode)
{
    m_numSprites = numSprites;
    m_sprites    = new const Sprite*[(unsigned long)m_numSprites];
    for (int i = sIndex, j = 0; i < sIndex + numSprites; ++i, ++j)
    {
        m_sprites[j] = &spriteSheet->GetSprite(i);
    }
    m_duration = duration;
    m_playMode = playMode;
}

Animation::~Animation() { delete[] m_sprites; }

int Animation::GetCurrentIndex(float elapsedTime) const
{
    if (m_numSprites == 1)
    {
        return 0;
    }
    int index = elapsedTime / m_duration;
    switch (m_playMode)
    {
    case PLAY_MODE_NORMAL: index = Math::Min(index, m_numSprites - 1); break;
    case PLAY_MODE_LOOP: index = index % m_numSprites; break;
    case PLAY_MODE_LOOP_PINGPONG:
        index = index % ((m_numSprites * 2) - 2);
        if (index > m_numSprites)
        {
            index = m_numSprites - 2 - (index - m_numSprites);
        }
        break;
    case PLAY_MODE_LOOP_REVERSE:
        index = (m_numSprites - 1) - index % m_numSprites;
        break;
    }

    m_lastIndex = index;
    m_lastTime  = elapsedTime;
    return index;
}

const Sprite* Animation::GetCurrentSprite(float elapsedTime) const
{
    return m_sprites[GetCurrentIndex(elapsedTime)];
}

void Animation::SetSprites(const Sprite* sprites[], int numSprites)
{
    if (m_sprites != nullptr)
    {
        delete[] m_sprites;
        m_sprites    = nullptr;
        m_numSprites = 0;
    }
    m_numSprites = numSprites;
    m_sprites    = new const Sprite*[(unsigned long)m_numSprites];
    for (int i = 0; i < m_numSprites; ++i)
    {
        m_sprites[i] = sprites[i];
    }
}

bool Animation::IsFinished(float elapsedTime) const
{
    int index = elapsedTime / m_duration;
    return index >= m_numSprites - 1;
}
