#ifndef OBJECT_LAYER_H
#define OBJECT_LAYER_H
#include "Layer.h"
class GameObject;
class ObjectLayer : public Layer
{
  public:
    ObjectLayer();
    ~ObjectLayer();

    void Tick(float deltaTime) override;
    void Paint() override;
	void Start() override;

    GameObject* GetObjectAt(int index) const;
    void        AddObject(GameObject* obj);
    int         IndexOf(GameObject* obj) const;
    int         GetNumObjects() const;

  private:
    void         GrowIfNeed();
    void         SwapAndPop(int i);
    void         Refresh();
    int          SortRemovedObjects();
    GameObject** m_objects;
    int          m_numObjects;
    int          m_capacity;
    bool         m_isStarted;
};
#endif // OBJECT_LAYER_H
