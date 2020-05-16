#include "GameObject.h"
#include "ObjectLayer.h"
#include "Vec.h"

GameObject::GameObject() :
    m_objectLayer(nullptr), m_positionX(0), m_positionY(0), m_rotation(0.0),
    m_isVisible(true), m_isActive(true), m_needToRemove(false)
{
}

GameObject::~GameObject() {}

void GameObject::Cleanup() { m_objectLayer = nullptr; }

void GameObject::Start() {}

float GameObject::GetPositionX() const { return m_positionX; }

void GameObject::SetPositionX(float x)
{
    if (m_positionX != x)
    {
        m_positionX = x;
        OnPositionChanged();
    }
}

float GameObject::GetPositionY() const { return m_positionY; }

void GameObject::SetPositionY(float y)
{
    if (m_positionY != y)
    {
        m_positionY = y;
        OnPositionChanged();
    }
}

Vec2 GameObject::GetPosition() const { return {m_positionX, m_positionY}; }

void GameObject::SetPosition(float x, float y)
{
    if (m_positionX != x || m_positionY != y)
    {
        m_positionX = x;
        m_positionY = y;
        OnPositionChanged();
    }
}

void   GameObject::SetPosition(const Vec2& pos) { SetPosition(pos.x, pos.y); }
double GameObject::GetRotation() const { return m_rotation; }

void GameObject::SetRotation(double rotation)
{
    if (m_rotation != rotation)
    {
        m_rotation = rotation;
        OnRotationChanged();
    }
}

bool GameObject::IsVisible() const { return m_isVisible; }

void GameObject::Remove() { m_needToRemove = true; }

void GameObject::SetIsVisible(bool flag)
{
    if (m_isVisible != flag)
    {
        m_isVisible = flag;
        flag ? OnBecomeVisible() : OnBecomeInvisible();
    }
}

void GameObject::Show()
{
    if (!m_isVisible)
    {
        m_isVisible = true;
        OnBecomeVisible();
    }
}

void GameObject::Hide()
{
    if (m_isVisible)
    {
        m_isVisible = false;
        OnBecomeInvisible();
    }
}

bool GameObject::IsActive() const { return m_isActive; }

void GameObject::SetIsActive(bool flag)
{
    if (m_isActive != flag)
    {
        m_isActive = flag;
        flag ? OnAcivated() : OnDeactivated();
    }
}

ObjectLayer* GameObject::GetObjectLayer() const { return m_objectLayer; }

void GameObject::SetObjectLayer(ObjectLayer* objectLayer)
{
    m_objectLayer = objectLayer;
}

Scene* GameObject::GetScene() const
{
    return m_objectLayer->GetScene();
}

void GameObject::OnPositionChanged() {}
void GameObject::OnRotationChanged() {}
void GameObject::OnBecomeVisible() {}
void GameObject::OnBecomeInvisible() {}
void GameObject::OnAcivated() {}
void GameObject::OnDeactivated() {}
