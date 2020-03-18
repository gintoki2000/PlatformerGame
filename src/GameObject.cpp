#include "GameObject.h"
#include "Level.h"
#include "NTRect.h"

GameObject::GameObject(int gameObjectType) :
    m_level(nullptr),
    m_positionX(0),
    m_positionY(0),
    m_rotation(0.0),
    m_isVisible(true),
    m_type(gameObjectType),
    m_next(nullptr),
    m_prev(nullptr)
{
}
int GameObject::getPositionX() const { return m_positionX; }
void GameObject::setPositionX(int x)
{
    if (m_positionX != x)
    {
        m_positionX = x;
        onPositionChanged();
    }
}

int GameObject::getPositionY() const { return m_positionY; }
void GameObject::setPositionY(int y)
{
    if (m_positionY != y)
    {
        m_positionY = y;
        onPositionChanged();
    }
}

void GameObject::setPosition(int x, int y)
{
    if (m_positionX != x || m_positionY != y)
    {
        m_positionX = x;
        m_positionY = y;
        onPositionChanged();
    }
}

Level* GameObject::getLevel() const { return m_level; }

void GameObject::setLevel(Level* level) { m_level = level; }

bool GameObject::isVisible() const { return m_isVisible; }
void GameObject::setVisible(bool visible) { m_isVisible = visible; }
