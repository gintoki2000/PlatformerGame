#include "Camera.h"
#include "Math.h"
Camera::Camera()
{

}

Camera::Camera(const Vec2 &center, const Vec2 &size):
    m_center(center), m_size(size), m_target(center)
{
    m_needToUpdateViewport = true;
}

void Camera::Update()
{
    m_center.x += Math::Lerp(m_center.x, m_target.x, 0.1f);
    m_center.y += Math::Lerp(m_center.y, m_target.y, 0.1f);
    m_needToUpdateViewport = true;
}

void Camera::SetCenter(const Vec2 &center)
{
    m_needToUpdateViewport = true;
    m_center = center;
}

Vec2 Camera::GetCenter() const
{
    return m_center;
}

Vec2 Camera::GetTarget() const
{
    return m_target;
}

void Camera::SetTarget(const Vec2 &target)
{
    m_target = target;
}

Vec2 Camera::GetSize() const
{
    return m_size;
}

void Camera::SetSize(const Vec2 &size)
{
    m_size = size;
}

Rect Camera::GetViewport() const
{
    if (m_needToUpdateViewport)
    {
        m_viewport.x = (int)(m_center.x - m_size.x / 2.f);
        m_viewport.y = (int)(m_center.y - m_size.y / 2.f);
        m_viewport.w = (int)m_size.x;
        m_viewport.h = (int)m_size.y;
        m_needToUpdateViewport = false;
    }
    return m_viewport;
}
