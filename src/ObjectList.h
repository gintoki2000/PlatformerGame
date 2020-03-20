#ifndef OBJECT_LIST
#define OBJECT_LIST
#include "NTRect.h"
#include "SDL.h"
class GameObject;
class ObjectList
{
  public:
    ObjectList();

    virtual ~ObjectList();

    void addObject(GameObject* gameObject);

    void removeObject(GameObject* gameObject);

    void tick(float deltaTime);
    void paint();

    int getNumObjects() const { return m_numObjects; }

  private:
    GameObject* m_list;
    int         m_numObjects;
};
#endif // OBJECT_LIST
