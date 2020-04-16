#include "PositionalLayer.h"

PositionalLayer::PositionalLayer() : m_position(0.f, 0.f) {}

const Vec2& PositionalLayer::getPosition() const { return m_position; }
void        PositionalLayer::setPosition(const Vec2& position)
{
    m_position = position;
}

float PositionalLayer::getPositionX() const { return m_position.x; }
void  PositionalLayer::setPositionX(float positionX)
{
    m_position.x = positionX;
}
float PositionalLayer::getPositionY() const { return m_position.y; }
void  PositionalLayer::setPositionY(float positionY)
{
    m_position.y = positionY;
}

void PositionalLayer::moveBy(const Vec2 &v)
{
	m_position += v;
}
