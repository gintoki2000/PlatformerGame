#include "SpriteSheet.h"
#include "SDL_render.h"

SpriteSheet::SpriteSheet(SDL_Texture* texture, int spriteWidth,
                         int spriteHeight)
{
    int textureWidth, textureHeight, cols, rows;
    SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);
    rows = textureHeight / spriteHeight;
    cols = textureWidth / spriteWidth;
    m_sprites = new Sprite[rows * cols];
    SDL_Rect rect;
    rect.w = spriteWidth;
    rect.h = spriteHeight;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            rect.x = j * spriteWidth;
            rect.y = i * spriteHeight;
            int index = i * cols + j;
            m_sprites[index] = Sprite(texture, rect);
        }
    }
	m_size = rows * cols;
}

SpriteSheet::~SpriteSheet() { delete[] m_sprites; }

const Sprite& SpriteSheet::getSprite(int index) const
{
    SDL_assert(index >= 0 || index < m_size);
    return m_sprites[index];
}
