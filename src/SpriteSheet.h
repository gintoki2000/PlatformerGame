#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H
#include "NTTextureRegion.h"
#include "SDL_render.h"
typedef NTTextureRegion Sprite;
class SpriteSheet 
{
	public:
			SpriteSheet(SDL_Texture* texture, int spriteWidth, int spriteHeight);

			~SpriteSheet();

			int getSize() const { return m_size; }

			const Sprite&  getSprite(int index) const;
	private:
			Sprite* m_sprites;
			int m_size;
};
#endif //SPRITE_SHEET_H
