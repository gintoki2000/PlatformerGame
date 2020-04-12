#include "GameObject.h"
#include "NTRect.h"

GameObject::GameObject(Level *level)
    : m_level(level),m_positionX(0), m_positionY(0), m_rotation(0.0),
      m_isVisible(true), m_isAlive(true) {}

GameObject::~GameObject() {}

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

Level *GameObject::getLevel() const { return m_level; }

void GameObject::setLevel(Level *level) { m_level = level; }

bool GameObject::isVisible() const { return m_isVisible; }

void GameObject::setVisible(bool visible) { m_isVisible = visible; }

bool GameObject::isAlive() const
{
    return m_isAlive;
}

void GameObject::setIsAlive(bool isAlive)
{
    m_isAlive = isAlive;
}


