#ifndef NT_LAYER_H
#define NT_LAYER_H
#include "NTRect.h"
#include "SDL.h"
class NTLayer 
{
		NTLayer();

		virtual ~NTLayer();

		virtual void update(float dt);

		virtual void draw(SDL_Renderer* renderer, const NTRect& viewPort) = 0;
protected:
		int m_positionX;
		int m_positionY;
};
#endif //NT_LAYER_H
