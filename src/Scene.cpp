#include "Scene.h"
#include "Layer.h"
#include "SDL_assert.h"

Scene::Scene()
{
    m_numLayers = 0;
    m_capacity  = 16;
    m_layers    = new Layer*[(unsigned)m_capacity];
    m_isStarted = false;
}

Scene::~Scene()
{
    for (int i = 0; i < m_numLayers; ++i)
    {
        delete m_layers[i];
        m_layers[i] = nullptr;
    }
    delete[] m_layers;
}

void Scene::update(float deltaTime)
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

void Scene::render()
{
    for (int i = 0; i < m_numLayers; ++i)
    {
        if (m_layers[i]->isVisible())
        {
            m_layers[i]->render();
        }
    }
}

void Scene::start()
{
    if (!m_isStarted)
    {
        m_isStarted = true;
        for (int i = 0; i < m_numLayers; ++i)
        {
            m_layers[i]->start();
        }
    }
}

int    Scene::getNumLayers() const { return m_numLayers; }
Layer* Scene::getLayerAt(int index) const
{
    SDL_assert(index >= 0 && index < m_numLayers);
    return m_layers[index];
}

Layer* Scene::getLayerByName(const char* layerName)
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

int Scene::indexOf(Layer* layer) const
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

void Scene::addLayer(Layer* layer)
{
    SDL_assert(layer != nullptr && layer->getScene() == nullptr);
    growIfNeed();
    m_layers[m_numLayers++] = layer;
    layer->setScene(this);
}

void Scene::addLayer(Layer* layer, int index)
{
    SDL_assert(index >= 0 && index < m_numLayers);
    SDL_assert(layer != nullptr && layer->getScene() == nullptr);
    growIfNeed();
    for (int i = m_numLayers; i > index; --i)
    {
        m_layers[i] = m_layers[i - 1];
    }
    m_layers[index] = layer;
    layer->setScene(this);
    ++m_numLayers;
}

bool Scene::removeLayer(Layer* layer)
{
    int index = indexOf(layer);
    if (index == -1)
        return false;

    removeLayerAt(index);
    return true;
}

Layer* Scene::removeLayerAt(int index)
{
    Layer* layer = getLayerAt(index);
    for (int i = index; i < m_numLayers - 1; ++i)
    {
        m_layers[i] = m_layers[i + 1];
    }
    layer->setScene(nullptr);
    return layer;
}

Camera& Scene::getCamera() { return m_camera; }

void Scene::growIfNeed()
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
