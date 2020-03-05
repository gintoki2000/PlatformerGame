#ifndef OBJECT_LAYER_H
#define OBJECT_LAYER_H
#include "SDL.h"
#include "NTRect.h"
class GameObject;
class ObjectLayer
{
	public:
		ObjectLayer();

		void addObject(GameObject* gameObject);

		void removeObject(GameObject* gameObject);

		virtual void update(float dt);

		virtual void paint(SDL_Renderer* renderer, const NTRect& viewPort);
	private:
		GameObject* m_list;
};
#endif //OBJECT_LAYER_H
