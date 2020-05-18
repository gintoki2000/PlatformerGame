#include "ObjectLayer.h"
#include "GameObject.h"
#include "SDL_assert.h"

ObjectLayer::ObjectLayer() :
    m_objects(nullptr), m_numObjects(0), m_capacity(16), m_isStarted(false)
{
    m_objects = new GameObject*[(unsigned)m_capacity];
}

ObjectLayer::~ObjectLayer()
{
    for (int i = 0; i < m_numObjects; ++i)
    {
        m_objects[i]->Cleanup();
    }
    delete[] m_objects;
}

void ObjectLayer::Tick(float deltaTime)
{
    Refresh();
    int numObjects = m_numObjects;
    for (int i = 0; i < numObjects; ++i)
    {
        if (m_objects[i]->IsActive())
        {
            m_objects[i]->Tick(deltaTime);
        }
    }
}

void ObjectLayer::Paint()
{
    int numObjects = m_numObjects;
    for (int i = 0; i < numObjects; ++i)
    {
        if (m_objects[i]->IsVisible())
        {
            m_objects[i]->Paint();
        }
    }
}

void ObjectLayer::Start()
{
    if (!m_isStarted)
    {
        for (int i = 0; i < m_numObjects; ++i)
        {
            m_objects[i]->Start();
        }
        m_isStarted = true;
    }
}

GameObject* ObjectLayer::GetObjectAt(int index) const
{
    return m_objects[index];
}

void ObjectLayer::AddObject(GameObject* obj)
{
    SDL_assert(obj != nullptr && obj->GetObjectLayer() == nullptr);
    GrowIfNeed();
    m_objects[m_numObjects++] = obj;
    obj->SetObjectLayer(this);
    if (m_isStarted)
    {
        obj->Start();
    }
}

int ObjectLayer::IndexOf(GameObject* obj) const
{
    for (int i = 0; i < m_numObjects; ++i)
    {
        if (m_objects[i] == obj)
        {
            return i;
        }
    }
    return -1;
}

int ObjectLayer::GetNumObjects() const { return m_numObjects; }

void ObjectLayer::GrowIfNeed()
{
    if (m_numObjects == m_capacity)
    {
        m_capacity += 16;
        GameObject** newArray = new GameObject*[(unsigned)m_capacity];
        SDL_memcpy(newArray, m_objects,
                   (unsigned)m_numObjects * sizeof(GameObject*));
        delete[] m_objects;
        m_objects = newArray;
    }
}

void ObjectLayer::Refresh()
{
    int i = 0;
    while (i < m_numObjects)
    {
        if (m_objects[i]->m_needToRemove)
        {
            m_objects[i]->SetObjectLayer(nullptr);
            m_objects[i]->m_needToRemove = false;
            m_objects[i]->Cleanup();

            m_objects[i] = m_objects[m_numObjects - 1];
			--m_numObjects;
        }
        else
        {
            ++i;
        }
    }
}
