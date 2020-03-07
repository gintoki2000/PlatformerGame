#ifndef OBJECT_LAYER_H
#define OBJECT_LAYER_H
#include "NTRect.h"
#include "SDL.h"
class GameObject;
class ObjectLayer
{
  public:
    ObjectLayer();

    virtual ~ObjectLayer();

    void addObject(GameObject* gameObject);

    void removeObject(GameObject* gameObject);

    virtual void update(float dt);

    virtual void draw(SDL_Renderer* renderer, const NTRect& viewPort);

    int getNumObjects() const { return m_numObjects; }

  private:
    GameObject* m_list;
    int m_numObjects;
};
#endif // OBJECT_LAYER_H
