#include "Layer.h"

Layer::Layer() : m_isVisible(true), m_isActive(true), m_manager(nullptr) {}

void Layer::SetIsActive(bool isActive) { m_isActive = isActive; }

Scene* Layer::GetScene() const { return m_manager; }

void Layer::SetScene(Scene* manager) { m_manager = manager; }

const std::string& Layer::GetName() const { return m_name; }

void Layer::SetName(const std::string& name) { m_name = name; }

void Layer::SetIsVisible(bool isVisible)
{
    if (m_isVisible != isVisible)
    {
        m_isVisible = isVisible;
        m_isVisible ? OnBecomeVisible() : OnBecomeInvisible();
    }
}

void Layer::Show()
{
    if (!m_isVisible)
    {
        m_isVisible = true;
        OnBecomeVisible();
    }
}

void Layer::Hide()
{
    if (m_isVisible)
    {
        m_isVisible = false;
        OnBecomeInvisible();
    }
}

Layer::~Layer() {}

void Layer::OnBecomeVisible() {}
void Layer::OnBecomeInvisible() {}
void Layer::Start() {}
