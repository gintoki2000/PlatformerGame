#include "SpriteSheet.h"
#include "SDL_render.h"

SpriteSheet::SpriteSheet(SDL_Texture* texture, int spriteWidth,
                         int spriteHeight)
{
    int textureWidth, textureHeight, cols, rows;
    SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);
    rows = textureHeight / spriteHeight;
    cols = textureWidth / spriteWidth;
    m_sprites = new Sprite[(unsigned long)(rows * cols)];
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
    m_numSprites = rows * cols;
}

SpriteSheet::~SpriteSheet() { delete[] m_sprites; }

const Sprite& SpriteSheet::getSprite(int index) const
{
    SDL_assert(index >= 0 || index < m_numSprites);
    return m_sprites[index];
}

void SpriteSheet::draw(SDL_Renderer* renderer, int index,
                       const SDL_Rect* dstrect) const
{
    getSprite(index).draw(renderer, dstrect);
}

void SpriteSheet::draw(SDL_Renderer* renderer, int index,
                       const SDL_Rect* dstrect, double angle,
                       const SDL_Point* center, SDL_RendererFlip flip) const
{
    getSprite(index).draw(renderer, dstrect, angle, center, flip);
}

void SpriteSheet::draw(SDL_Renderer* renderer, int index, int x, int y) const
{
    getSprite(index).draw(renderer, x, y);
}

void SpriteSheet::draw(SDL_Renderer* renderer, int index, int x, int y,
                       SDL_RendererFlip flip) const
{
    getSprite(index).draw(renderer, x, y, flip);
}

void SpriteSheet::getSprites(std::vector<Sprite> &sprites, int sIndex, int count)
{
	sprites.reserve((std::size_t) count);
	for (int i = sIndex, j = 0; i < sIndex + count; ++i, ++j)
	{
		sprites[(std::size_t) j] = m_sprites[i];	
	}
}
