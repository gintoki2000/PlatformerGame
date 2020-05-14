#include "Layer.h"

Layer::Layer() : m_isVisible(true), m_isActive(true), m_manager(nullptr) {}

void Layer::setIsActive(bool isActive) { m_isActive = isActive; }

Scene* Layer::getScene() const { return m_manager; }

void Layer::setScene(Scene* manager) { m_manager = manager; }

const std::string& Layer::getName() const { return m_name; }

void Layer::setName(const std::string& name) { m_name = name; }

void Layer::setIsVisible(bool isVisible)
{
    if (m_isVisible != isVisible)
    {
        m_isVisible = isVisible;
        m_isVisible ? onBecomeVisible() : onBecomeInvisible();
    }
}

void Layer::show()
{
    if (!m_isVisible)
    {
        m_isVisible = true;
        onBecomeVisible();
    }
}

void Layer::hide()
{
    if (m_isVisible)
    {
        m_isVisible = false;
        onBecomeInvisible();
    }
}

Layer::~Layer() {}

void Layer::onBecomeVisible() {}
void Layer::onBecomeInvisible() {}
void Layer::start() {}
