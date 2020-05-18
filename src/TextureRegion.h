#ifndef TEXTURE_REGION_H
#define TEXTURE_REGION_H
#include "SDL2/SDL.h"
#include "SDL_render.h"
struct TextureRegion
{
    SDL_Texture* texture;
    SDL_Rect     rect;

    TextureRegion();
    TextureRegion(SDL_Texture* _texture);
    TextureRegion(SDL_Texture* _texture, const SDL_Rect& _rect);

    int Draw(SDL_Renderer* renderer, const SDL_Rect* dstrect) const;
    int Draw(SDL_Renderer* renderer, const SDL_Rect* dstrect, double angle,
             const SDL_Point* center, SDL_RendererFlip flip) const;
    int Draw(SDL_Renderer* renderer, int x, int y) const;
    int Draw(SDL_Renderer* renderer, int x, int y, SDL_RendererFlip flip) const;
    int GetWidth() const { return rect.w; }
    int GetHeight() const { return rect.h; }
};
#endif // TEXTURE_REGION_H
