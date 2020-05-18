#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H
#include "SDL_render.h"
#include "TextureRegion.h"
typedef TextureRegion Sprite;
class SpriteSheet
{
  public:
    SpriteSheet();

    void Init(SDL_Texture* texture, int spriteWidth, int spriteHeight);

    void Init(SDL_Texture* texture, int startX, int startY, int rows, int cols,
              int spriteWidth, int spriteHeight);

    SpriteSheet(const SpriteSheet&);

    SpriteSheet& operator=(const SpriteSheet&);

    ~SpriteSheet();

    int GetNumSprites() const { return m_numSprites; }

    const Sprite& GetSprite(int index) const;

    void Draw(SDL_Renderer* renderer, int index, const SDL_Rect* dstrect) const;

    void Draw(SDL_Renderer* renderer, int index, const SDL_Rect* dstrect,
              double angle, const SDL_Point* center,
              SDL_RendererFlip flip) const;

    void Draw(SDL_Renderer* renderer, int index, int x, int y) const;

    void Draw(SDL_Renderer* renderer, int index, int x, int y,
              SDL_RendererFlip flip) const;

  private:
    Sprite* m_sprites;
    int     m_numSprites;
};
#endif // SPRITE_SHEET_H
