#include "NTTextureRegion.h"
#include "SDL_render.h"

NTTextureRegion::NTTextureRegion() : NTTextureRegion(nullptr, {}) {}

NTTextureRegion::NTTextureRegion(SDL_Texture *_texture) : texture(_texture) {
  rect.x = 0;
  rect.y = 0;
  SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
}

NTTextureRegion::NTTextureRegion(SDL_Texture *_texture, const SDL_Rect &_rect)
    : texture(_texture), rect(_rect) {}

int NTTextureRegion::draw(SDL_Renderer *renderer,
                          const SDL_Rect *dstrect) const {
  return SDL_RenderCopy(renderer, texture, &rect, dstrect);
}

int NTTextureRegion::draw(SDL_Renderer *renderer, const SDL_Rect *dstrect,
                          double angle, const SDL_Point *center,
                          SDL_RendererFlip flip) const {
  return SDL_RenderCopyEx(renderer, texture, &rect, dstrect, angle, center,
                          flip);
}

int NTTextureRegion::draw(SDL_Renderer* renderer, int x, int y) const
{
	SDL_Rect dstrect;
	dstrect.x = x;
	dstrect.y = y;
	dstrect.w = rect.w;
	dstrect.h = rect.h;
	return SDL_RenderCopy(renderer, texture, &rect, &dstrect);
}
