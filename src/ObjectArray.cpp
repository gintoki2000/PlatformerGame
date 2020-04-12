#include "ObjectArray.h"
#include "GameObject.h"
ObjectManager::ObjectManager(): ObjectManager(16)
{

}

ObjectManager::ObjectManager(int initialCapacity)
{
    m_objects = new GameObject*[(unsigned) initialCapacity];
    m_capacity = initialCapacity;
    m_numObjects = 0;
    m_numObjectsNext = 0;
}

ObjectManager::~ObjectManager()
{

}

GameObject *ObjectManager::getObjectAt(int i) const
{
    return m_objects[i];
}

int ObjectManager::getNumObjects() const
{
    return m_numObjects;
}

int ObjectManager::getCapacity() const
{
    return m_capacity;
}

void ObjectManager::addObject(GameObject *newObject)
{
    growIfNeed();
    m_objects[m_numObjectsNext++] = newObject;
}

void ObjectManager::refresh()
{
    if (m_numObjectsNext == 0)
    {
        m_numObjects = 0;
        return;
    }
    m_numObjects = _refresh();
    for (int i = m_numObjects; i < m_numObjectsNext; ++i)
    {
        delete m_objects[i];
    }
    m_numObjectsNext = m_numObjects;
}

void ObjectManager::tick(float deltaTime)
{
    for (int i = 0; i < m_numObjects; ++i)
    {
        if (m_objects[i]->isAlive())
        {
            m_objects[i]->tick(deltaTime);
        }
    }
}

void ObjectManager::paint()
{
    for (int i = 0; i < m_numObjects; ++i)
    {
        if (m_objects[i]->isVisible())
        {
            m_objects[i]->paint();
        }
    }
}

int ObjectManager::_refresh()
{
    int iA = m_numObjectsNext - 1;
    int iD = 0;
    while (iA > iD)
    {
        for(; true; ++iD)
        {
            if (iD > iA) return iD;
            if (m_objects[iD]->isAlive()) break;
        }
        for (; true; --iA)
        {
            if (!m_objects[iA]->isAlive()) break;
            if (iA <= iD) return iA;
        }
        GameObject* tmp = m_objects[iA];
        m_objects[iA] = m_objects[iD];
        m_objects[iD] = tmp;
    }
    return iD;
}

void ObjectManager::growIfNeed()
{
    if (m_numObjectsNext == m_capacity)
    {
        GameObject** newArray = new GameObject*[(unsigned)m_capacity * 2];
        SDL_memcpy(newArray, m_objects, (unsigned)m_numObjectsNext * sizeof(GameObject*));
        delete[] m_objects;
        m_objects = newArray;
    }
}





