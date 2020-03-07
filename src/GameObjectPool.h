#ifndef GAME_OBJECT_POOL_H
#define GAME_OBJECT_POOL_H
#include "SDL.h"
#include <cassert>
#include <new>
template <class T, class Y> class GameObjectPool
{
  public:
    ~GameObjectPool() { delete[] m_objects; }

    static GameObjectPool* create(int numObjects)
    {
        GameObjectPool* ret = new (std::nothrow) GameObjectPool;
        if (ret && ret->initialize(numObjects))
        {
            return ret;
        }
        delete ret;
        return nullptr;
    }

    T* getObject()
    {
        assert(m_firstAvailabe != nullptr &&
               "No object available in this pool");
        T* ret = m_firstAvailabe;
        m_firstAvailabe = static_cast<T*>(m_firstAvailabe->getNext());
        ret->setNext(nullptr);
        return ret;
    }

    void releaseObject(T* gameObject)
    {
        gameObject->setNext(m_firstAvailabe);
        m_firstAvailabe = gameObject;
    }

    const Y& getAsserts() const { return m_asserts; }

  private:
    GameObjectPool() :
        m_objects(nullptr), m_firstAvailabe(nullptr), m_numObjects(0){};

    bool initialize(int numObjects)
    {
        m_numObjects = numObjects;
        if (!m_asserts.load())
        {
            SDL_Log("Failed to load asserts!");
            return false;
        }
        m_objects = new T[m_numObjects];
        for (int i = 0; i < m_numObjects; ++i)
        {
            m_objects[i].create(this, m_asserts);
        }
        for (int i = 0; i < m_numObjects - 1; ++i)
        {
            m_objects[i].setNext(&m_objects[i + 1]);
        }
        m_objects[m_numObjects - 1].setNext(nullptr);
        m_firstAvailabe = &m_objects[0];
        return true;
    }

    T* m_objects;
    Y m_asserts;
    T* m_firstAvailabe;
    int m_numObjects;
};
#endif // GAME_OBJECT_POOL_H
