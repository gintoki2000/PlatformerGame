#include "SpriteSheet.h"
#include "SDL_assert.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"

SpriteSheet::SpriteSheet()
{
    m_sprites    = nullptr;
    m_numSprites = 0;
}

SpriteSheet& SpriteSheet::operator=(const SpriteSheet& other)
{
    this->~SpriteSheet();

    m_sprites    = new Sprite[other.m_numSprites];
    m_numSprites = other.m_numSprites;
    SDL_memcpy(m_sprites, other.m_sprites, m_numSprites * sizeof(Sprite));

    return *this;
}

SpriteSheet::SpriteSheet(const SpriteSheet& other)
{
    this->~SpriteSheet();

    m_sprites    = new Sprite[other.m_numSprites];
    m_numSprites = other.m_numSprites;
    SDL_memcpy(m_sprites, other.m_sprites, m_numSprites * sizeof(Sprite));
}
void SpriteSheet::Init(SDL_Texture* texture, int spriteWidth, int spriteHeight)
{
    SDL_assert(texture != nullptr);
    SDL_assert(m_sprites == nullptr);
    int textureWidth, textureHeight, cols, rows;
    SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);
    rows      = textureHeight / spriteHeight;
    cols      = textureWidth / spriteWidth;
    m_sprites = new Sprite[(unsigned long)(rows * cols)];
    SDL_Rect rect;
    rect.w = spriteWidth;
    rect.h = spriteHeight;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            rect.x           = j * spriteWidth;
            rect.y           = i * spriteHeight;
            int index        = i * cols + j;
            m_sprites[index] = Sprite(texture, rect);
        }
    }
    m_numSprites = rows * cols;
}

void SpriteSheet::Init(SDL_Texture* texture, int startX, int startY, int rows, int cols,
              int spriteWidth, int spriteHeight)
{
	
    SDL_assert(texture != nullptr);
    SDL_assert(m_sprites == nullptr);
    m_sprites = new Sprite[(unsigned long)(rows * cols)];
    SDL_Rect rect;
    rect.w = spriteWidth;
    rect.h = spriteHeight;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            rect.x           = j * spriteWidth + startX;
            rect.y           = i * spriteHeight + startY;
            int index        = i * cols + j;
            m_sprites[index] = Sprite(texture, rect);
        }
    }
    m_numSprites = rows * cols;
}

SpriteSheet::~SpriteSheet()
{
    delete[] m_sprites;
    m_sprites = nullptr;
}

const Sprite& SpriteSheet::GetSprite(int index) const
{
    SDL_assert(index >= 0 || index < m_numSprites);
    return m_sprites[index];
}

void SpriteSheet::Draw(SDL_Renderer* renderer, int index,
                       const SDL_Rect* dstrect) const
{
    GetSprite(index).Draw(renderer, dstrect);
}

void SpriteSheet::Draw(SDL_Renderer* renderer, int index,
                       const SDL_Rect* dstrect, double angle,
                       const SDL_Point* center, SDL_RendererFlip flip) const
{
    GetSprite(index).Draw(renderer, dstrect, angle, center, flip);
}

void SpriteSheet::Draw(SDL_Renderer* renderer, int index, int x, int y) const
{
    GetSprite(index).Draw(renderer, x, y);
}

void SpriteSheet::Draw(SDL_Renderer* renderer, int index, int x, int y,
                       SDL_RendererFlip flip) const
{
    GetSprite(index).Draw(renderer, x, y, flip);
}
