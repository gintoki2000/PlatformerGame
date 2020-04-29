#include "GameObject.h"
#include "ObjectLayer.h"
#include "Vec.h"

GameObject::GameObject() :
    m_objectLayer(nullptr), m_positionX(0), m_positionY(0), m_rotation(0.0),
    m_isVisible(true), m_isActive(true), m_needToRemove(false)
{
}

GameObject::~GameObject() {}

void GameObject::cleanup() { m_objectLayer = nullptr; }

void GameObject::start() {}

float GameObject::getPositionX() const { return m_positionX; }

void GameObject::setPositionX(float x)
{
    if (m_positionX != x)
    {
        m_positionX = x;
        onPositionChanged();
    }
}

float GameObject::getPositionY() const { return m_positionY; }

void GameObject::setPositionY(float y)
{
    if (m_positionY != y)
    {
        m_positionY = y;
        onPositionChanged();
    }
}

Vec2 GameObject::getPosition() const { return {m_positionX, m_positionY}; }

void GameObject::setPosition(float x, float y)
{
    if (m_positionX != x || m_positionY != y)
    {
        m_positionX = x;
        m_positionY = y;
        onPositionChanged();
    }
}

void   GameObject::setPosition(const Vec2& pos) { setPosition(pos.x, pos.y); }
double GameObject::getRotation() const { return m_rotation; }

void GameObject::setRotation(double rotation)
{
    if (m_rotation != rotation)
    {
        m_rotation = rotation;
        onRotationChanged();
    }
}

bool GameObject::isVisible() const { return m_isVisible; }

void GameObject::remove() { m_needToRemove = true; }

void GameObject::setIsVisible(bool flag)
{
    if (m_isVisible != flag)
    {
        m_isVisible = flag;
        flag ? onBecomeVisible() : onBecomeInvisible();
    }
}

void GameObject::show()
{
    if (!m_isVisible)
    {
        m_isVisible = true;
        onBecomeVisible();
    }
}

void GameObject::hide()
{
    if (m_isVisible)
    {
        m_isVisible = false;
        onBecomeInvisible();
    }
}

bool GameObject::isActive() const { return m_isActive; }

void GameObject::setIsActive(bool flag)
{
    if (m_isActive != flag)
    {
        m_isActive = flag;
        flag ? onAcivated() : onDeactivated();
    }
}

ObjectLayer* GameObject::getObjectLayer() const { return m_objectLayer; }

void GameObject::setObjectLayer(ObjectLayer* objectLayer)
{
    m_objectLayer = objectLayer;
}

LayerManager* GameObject::getLayerManager() const
{
    return m_objectLayer->getManager();
}

void GameObject::onPositionChanged() {}
void GameObject::onRotationChanged() {}
void GameObject::onBecomeVisible() {}
void GameObject::onBecomeInvisible() {}
void GameObject::onAcivated() {}
void GameObject::onDeactivated() {}
