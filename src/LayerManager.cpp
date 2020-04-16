#include "LayerManager.h"
#include "Layer.h"
#include "SDL_assert.h"

LayerManager::LayerManager()
{
    m_numLayers = 0;
    m_capacity  = 16;
    m_layers    = new Layer*[(unsigned)m_capacity];
}

LayerManager::~LayerManager() {
    for (int i = 0; i < m_numLayers; ++i)
    {
        delete m_layers[i];
        m_layers[i] = nullptr;
    }
    delete[] m_layers;
}

void LayerManager::update(float deltaTime)
{
    m_camera.update();
    for (int i = 0; i < m_numLayers; ++i)
    {
        if (m_layers[i]->isActive())
        {
            m_layers[i]->update(deltaTime);
        }
    }
}

void LayerManager::render()
{
    for (int i = 0; i < m_numLayers; ++i)
    {
        if (m_layers[i]->isVisible())
        {
            m_layers[i]->render();
        }
    }
}

int    LayerManager::getNumLayers() const { return m_numLayers; }
Layer* LayerManager::getLayerAt(int index) const
{
    SDL_assert(index >= 0 && index < m_numLayers);
    return m_layers[index];
}

Layer* LayerManager::getLayerByName(const char* layerName)
{
    for (int i = 0; i < m_numLayers; ++i)
    {
        if (m_layers[i]->getName() == layerName)
        {
            return m_layers[i];
        }
    }
    return nullptr;
}

int LayerManager::indexOf(Layer* layer) const
{
    for (int i = 0; i < m_numLayers; ++i)
    {
        if (m_layers[i] == layer)
        {
            return i;
        }
    }
    return -1;
}

void LayerManager::addLayer(Layer* layer)
{
    SDL_assert(layer != nullptr && layer->getManager() == nullptr);
    growIfNeed();
    m_layers[m_numLayers++] = layer;
    layer->setManager(this);
}

void LayerManager::addLayer(Layer* layer, int index)
{
    SDL_assert(index >= 0 && index < m_numLayers);
    SDL_assert(layer != nullptr && layer->getManager() == nullptr);
    growIfNeed();
    for (int i = m_numLayers; i > index; --i)
    {
        m_layers[i] = m_layers[i - 1];
    }
    m_layers[index] = layer;
    layer->setManager(this);
    ++m_numLayers;
}

bool LayerManager::removeLayer(Layer* layer)
{
    int index = indexOf(layer);
    if (index == -1)
        return false;

    removeLayerAt(index);
    return true;
}

Layer* LayerManager::removeLayerAt(int index)
{
    Layer* layer = getLayerAt(index);
    for (int i = index; i < m_numLayers - 1; ++i)
    {
        m_layers[i] = m_layers[i + 1];
    }
    layer->setManager(nullptr);
    return layer;
}

Camera &LayerManager::getCamera()
{
   return m_camera;
}

void LayerManager::growIfNeed()
{
    if (m_numLayers == m_capacity)
    {
        m_capacity += 16;
        Layer** newArray = new Layer*[(unsigned)m_capacity];
        SDL_memcpy(newArray, m_layers, m_numLayers * sizeof(Layer*));
        delete[] m_layers;
        m_layers = newArray;
    }
}
