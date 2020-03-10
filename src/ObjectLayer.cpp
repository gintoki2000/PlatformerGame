#include "ObjectLayer.h"
#include "GameObject.h"
ObjectLayer::ObjectLayer() : m_list(nullptr), m_numObjects(0) {}

ObjectLayer::~ObjectLayer()
{
    GameObject* gameObject = m_list;
    GameObject* nextObject = nullptr;
    while (gameObject != nullptr)
    {
        nextObject = gameObject->getNext();
        gameObject->setPrev(nullptr);
        gameObject->setNext(nullptr);
		gameObject->cleanup();
        gameObject = nextObject;
    }
}

void ObjectLayer::addObject(GameObject* gameObject)
{
    if (m_list == nullptr)
    {
        m_list = gameObject;
    }
    else
    {
        gameObject->setNext(m_list);
        m_list->setPrev(gameObject);
        m_list = gameObject;
    }
	++m_numObjects;
}

void ObjectLayer::removeObject(GameObject* gameObject)
{
    if (gameObject == m_list)
        m_list = m_list->getNext();
    if (gameObject->getNext() != nullptr)
        gameObject->getNext()->setPrev(gameObject->getPrev());
    if (gameObject->getPrev() != nullptr)
        gameObject->getPrev()->setNext(gameObject->getNext());
    gameObject->setNext(nullptr);
    gameObject->setPrev(nullptr);
	gameObject->cleanup();
	--m_numObjects;
}

void ObjectLayer::update(float dt)
{
    for (GameObject* gameObject = m_list; gameObject != nullptr;
         gameObject = gameObject->getNext())
    {
        gameObject->update(dt);
    }
}

void ObjectLayer::draw(SDL_Renderer* renderer, const NTRect& viewPort)
{
    for (GameObject* gameObject = m_list; gameObject != nullptr;
         gameObject = gameObject->getNext())
    {
        gameObject->draw(renderer, viewPort);
    }
}
