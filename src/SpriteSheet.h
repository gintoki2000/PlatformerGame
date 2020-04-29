#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H
#include "SDL_render.h"
#include "TextureRegion.h"
typedef TextureRegion Sprite;
class SpriteSheet
{
  public:
    SpriteSheet();

    void init(SDL_Texture* texture, int spriteWidth, int spriteHeight);

    void init(SDL_Texture* texture, int startX, int startY, int rows, int cols,
              int spriteWidth, int spriteHeight);

    SpriteSheet(const SpriteSheet&);

    SpriteSheet& operator=(const SpriteSheet&);

    ~SpriteSheet();

    int getNumSprites() const { return m_numSprites; }

    const Sprite& getSprite(int index) const;

    void draw(SDL_Renderer* renderer, int index, const SDL_Rect* dstrect) const;

    void draw(SDL_Renderer* renderer, int index, const SDL_Rect* dstrect,
              double angle, const SDL_Point* center,
              SDL_RendererFlip flip) const;

    void draw(SDL_Renderer* renderer, int index, int x, int y) const;

    void draw(SDL_Renderer* renderer, int index, int x, int y,
              SDL_RendererFlip flip) const;

  private:
    Sprite* m_sprites;
    int     m_numSprites;
};
#endif // SPRITE_SHEET_H
