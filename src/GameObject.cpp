#include "GameObject.h"

float GameObject::getPositionX() const { return m_positionX; }
void GameObject::setPositionX(float x) { m_positionX = x; }

float GameObject::getPositionY() const { return m_positionY; }
void GameObject::setPositionY(float y) { m_positionY = y; }

void GameObject::setPosition(float x, float y)
{
	m_positionX = x;
	m_positionY = y;
}
