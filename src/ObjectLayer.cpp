#include "ObjectLayer.h"
#include "GameObject.h"
#include "SDL_assert.h"

ObjectLayer::ObjectLayer() : m_objects(nullptr), m_numObjects(0), m_capacity(16)
{
    m_objects = new GameObject*[(unsigned)m_capacity];
}

ObjectLayer::~ObjectLayer()
{
    for (int i = 0; i < m_numObjects; ++i)
    {
        delete m_objects[i];
    }
    delete[] m_objects;
}

void ObjectLayer::update(float deltaTime)
{
	refresh();
    int numObjects = m_numObjects;
    for (int i = 0; i < numObjects; ++i)
    {
        if (m_objects[i]->isActive())
        {
            m_objects[i]->tick(deltaTime);
        }
    }
}

void ObjectLayer::render()
{
    int numObjects = m_numObjects;
    for (int i = 0; i < numObjects; ++i)
    {
        if (m_objects[i]->isVisible())
        {
            m_objects[i]->paint();
        }
    }
}

GameObject *ObjectLayer::getObjectAt(int index) const
{
    SDL_assert(index > 0 && index < m_numObjects - 1);
    return m_objects[index];
}

void ObjectLayer::addObject(GameObject* obj)
{
    SDL_assert(obj != nullptr && obj->getObjectLayer() == nullptr);
    growIfNeed();
    m_objects[m_numObjects++] = obj;
    obj->setObjectLayer(this);
    obj->setLayerManager(getManager());
}

void ObjectLayer::removeObject(GameObject* obj)
{
    obj->scheduleRemove();
}

void ObjectLayer::removeObjectAt(int index)
{
    getObjectAt(index)->scheduleRemove();
}

int ObjectLayer::indexOf(GameObject* obj) const
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

int ObjectLayer::getNumObjects() const { return m_numObjects; }

void ObjectLayer::growIfNeed() 
{
	if (m_numObjects == m_capacity)
	{
		m_capacity += 16;
		GameObject** newArray = new GameObject*[(unsigned)m_capacity];
        SDL_memcpy(newArray, m_objects, (unsigned)m_numObjects * sizeof(GameObject*));
		delete[] m_objects;
		m_objects = newArray;
	}
}

void ObjectLayer::refresh()
{
	int i = 0;
	while(i < m_numObjects)
	{
		if (m_objects[i]->needToRemove())
		{
			m_objects[i]->setObjectLayer(nullptr);
			m_objects[i]->setLayerManager(nullptr);
			m_objects[i]->cleanup();

			m_objects[i] = m_objects[m_numObjects - 1];
		   	--m_numObjects;	
		}
		else 
		{
			++i;
		}
	}
}

