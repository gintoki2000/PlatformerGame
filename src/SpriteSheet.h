#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H
#include "TextureRegion.h"
#include "SDL_render.h"
typedef TextureRegion Sprite;
class SpriteSheet
{
  public:
    SpriteSheet(SDL_Texture* texture, int spriteWidth, int spriteHeight);

    SpriteSheet(const SpriteSheet&) = delete;

    SpriteSheet& operator=(const SpriteSheet&) = delete;

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
    int m_numSprites;
};
#endif // SPRITE_SHEET_H
