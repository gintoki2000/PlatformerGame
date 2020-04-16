#include "GameObject.h"

GameObject::GameObject()
    :m_objectLayer(nullptr), m_layerManager(nullptr), m_positionX(0), m_positionY(0), m_rotation(0.0),
      m_isVisible(true), m_isActive(true), m_needToRemove(false) {}

GameObject::~GameObject() {}

void GameObject::cleanup() { m_needToRemove = false; }

float GameObject::getPositionX() const { return m_positionX; }

void GameObject::setPositionX(float x) { m_positionX = x; }

float GameObject::getPositionY() const { return m_positionY; }

void GameObject::setPositionY(float y) { m_positionY = y; }

void GameObject::setPosition(float x, float y) {
  m_positionX = x;
  m_positionY = y;
}

double GameObject::getRotation() const { return m_rotation; }

void GameObject::setRotation(double rotation) { m_rotation = rotation; }


bool GameObject::isVisible() const { return m_isVisible; }

bool GameObject::needToRemove() const
{
    return m_needToRemove;
}

void GameObject::scheduleRemove()
{
    m_needToRemove = true;
}


void GameObject::setIsVisible(bool isVisible)
{
    m_isVisible = isVisible;
}

bool GameObject::isActive() const
{
    return m_isActive;
}

void GameObject::setIsActive(bool isActive)
{
    m_isActive = isActive;
}

void GameObject::setNeedToRemove(bool needToRemove)
{
    m_needToRemove = needToRemove;
}

ObjectLayer *GameObject::getObjectLayer() const
{
    return m_objectLayer;
}

void GameObject::setObjectLayer(ObjectLayer *objectLayer)
{
    m_objectLayer = objectLayer;
}

LayerManager *GameObject::getLayerManager() const
{
    return m_layerManager;
}

void GameObject::setLayerManager(LayerManager *layerManager)
{
    m_layerManager = layerManager;
}




