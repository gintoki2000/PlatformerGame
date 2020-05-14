#ifndef OBJECT_LAYER_H
#define OBJECT_LAYER_H
#include "Layer.h"
class GameObject;
class ObjectLayer : public Layer
{
  public:
    ObjectLayer();
    ~ObjectLayer();

    void update(float deltaTime) override;
    void render() override;
	void start() override;

    GameObject* getObjectAt(int index) const;
    void        addObject(GameObject* obj);
    int         indexOf(GameObject* obj) const;
    int         getNumObjects() const;

  private:
    void         growIfNeed();
    void         swapAndPop(int i);
    void         refresh();
    int          sortRemovedObjects();
    GameObject** m_objects;
    int          m_numObjects;
    int          m_capacity;
    bool         m_isStarted;
};
#endif // OBJECT_LAYER_H
