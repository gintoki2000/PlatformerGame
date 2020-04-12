#ifndef OBJECTARRAY_H
#define OBJECTARRAY_H

class GameObject;
class ObjectManager
{
public:
    ObjectManager();
    ObjectManager(int initialCapacity);
    ~ObjectManager();

    GameObject* getObjectAt(int i) const;
    int getNumObjects() const;
    int getCapacity() const;
    void addObject(GameObject* newObject);
    void refresh();
    void tick(float deltaTime);
    void paint();
private:

    int _refresh();
    void growIfNeed();

    GameObject** m_objects;
    int m_capacity;
    int m_numObjects;
    int m_numObjectsNext;
};

#endif // OBJECTARRAY_H
