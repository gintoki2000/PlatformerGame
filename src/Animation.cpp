#include "Animation.h"
#include "SpriteSheet.h"

Animation::Animation(const Sprite* sprites[], int numSprites, float duration, int playMode)
{
	m_numSprites = numSprites;
	m_sprites = new const Sprite*[(unsigned long)m_numSprites];		
	for (int i = 0; i < m_numSprites; ++i)
	{
		m_sprites[i] = sprites[i];
	}
	m_duration = duration;
	m_playMode = playMode;
}

Animation::Animation(const SpriteSheet* spriteSheet, float duration, int playMode):
		Animation(spriteSheet, 0, spriteSheet->getNumSprites(), duration, playMode)
{
}

Animation::Animation(const SpriteSheet* spriteSheet, int sIndex, int numSprites, float duration, int playMode)
{
	m_numSprites = numSprites;
	m_sprites = new const Sprite*[(unsigned long)m_numSprites];
	for (int i = sIndex, j = 0; i < sIndex + numSprites; ++i, ++j)
	{
		m_sprites[j] = &spriteSheet->getSprite(i);
	}
	m_duration = duration;
	m_playMode = playMode;
}

Animation::~Animation()
{
	delete[] m_sprites;
}
