#include "Layer.h"

Layer::Layer() : m_isVisible(true), m_isActive(true), m_manager(nullptr) {}


void Layer::setIsActive(bool isActive)
{
    m_isActive = isActive;
}

LayerManager *Layer::getManager() const
{
    return m_manager;
}

void Layer::setManager(LayerManager *manager)
{
    m_manager = manager;
}

std::string Layer::getName() const
{
    return m_name;
}

void Layer::setName(const std::string &name)
{
    m_name = name;
}

void Layer::setIsVisible(bool isVisible)
{
    m_isVisible = isVisible;
}

Layer::~Layer() {}
