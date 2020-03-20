#include "ObjectList.h"
#include "GameObject.h"
ObjectList::ObjectList() : m_list(nullptr), m_numObjects(0) {}

ObjectList::~ObjectList()
{
    GameObject* obj = m_list;
    GameObject* nextObj = nullptr;
    while (obj != nullptr)
    {
        nextObj = obj->m_next;
        delete obj;
        obj = nextObj;
    }
}

void ObjectList::addObject(GameObject* obj)
{
    if (m_list == nullptr)
    {
        m_list = obj;
    }
    else
    {
        obj->m_next = m_list;
        m_list->m_prev = obj;
        m_list = obj;
    }
    ++m_numObjects;
}

void ObjectList::removeObject(GameObject* obj)
{
    if (obj == m_list)
        m_list = m_list->m_next;
    if (obj->m_next != nullptr)
        obj->m_next->m_prev = obj->m_prev;
    if (obj->m_prev != nullptr)
        obj->m_prev->m_next = obj->m_next;
    --m_numObjects;
}

void ObjectList::tick(float deltaTime)
{
    for (auto obj = m_list; obj != nullptr; obj = obj->m_next)
    {
        obj->tick(deltaTime);
    }
}

void ObjectList::paint()
{

    for (auto obj = m_list; obj != nullptr; obj = obj->m_next)
    {
        obj->paint();
    }
}
