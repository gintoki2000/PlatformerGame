#include "PositionalLayer.h"

PositionalLayer::PositionalLayer() : m_position(0.f, 0.f) {}

const Vec2& PositionalLayer::GetPosition() const { return m_position; }
void        PositionalLayer::SetPosition(const Vec2& position)
{
    m_position = position;
}

float PositionalLayer::GetPositionX() const { return m_position.x; }
void  PositionalLayer::SetPositionX(float positionX)
{
    m_position.x = positionX;
}
float PositionalLayer::GetPositionY() const { return m_position.y; }
void  PositionalLayer::SetPositionY(float positionY)
{
    m_position.y = positionY;
}

void PositionalLayer::MoveBy(const Vec2 &v)
{
	m_position += v;
}
