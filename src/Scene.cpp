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

void Scene::Tick(float deltaTime)
{
    m_camera.Update();
    for (int i = 0; i < m_numLayers; ++i)
    {
        if (m_layers[i]->IsActive())
        {
            m_layers[i]->Tick(deltaTime);
        }
    }
}

void Scene::Paint()
{
    for (int i = 0; i < m_numLayers; ++i)
    {
        if (m_layers[i]->IsVisible())
        {
            m_layers[i]->Paint();
        }
    }
}

void Scene::Start()
{
    if (!m_isStarted)
    {
        m_isStarted = true;
        for (int i = 0; i < m_numLayers; ++i)
        {
            m_layers[i]->Start();
        }
    }
}

int    Scene::GetNumLayers() const { return m_numLayers; }
Layer* Scene::GetLayerAt(int index) const
{
    SDL_assert(index >= 0 && index < m_numLayers);
    return m_layers[index];
}

Layer* Scene::GetLayerByName(const char* layerName)
{
    for (int i = 0; i < m_numLayers; ++i)
    {
        if (m_layers[i]->GetName() == layerName)
        {
            return m_layers[i];
        }
    }
    return nullptr;
}

int Scene::IndexOf(Layer* layer) const
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

void Scene::AddLayer(Layer* layer)
{
    SDL_assert(layer != nullptr && layer->GetScene() == nullptr);
    GrowIfNeed();
    m_layers[m_numLayers++] = layer;
    layer->SetScene(this);
}

void Scene::AddLayer(Layer* layer, int index)
{
    SDL_assert(index >= 0 && index < m_numLayers);
    SDL_assert(layer != nullptr && layer->GetScene() == nullptr);
    GrowIfNeed();
    for (int i = m_numLayers; i > index; --i)
    {
        m_layers[i] = m_layers[i - 1];
    }
    m_layers[index] = layer;
    layer->SetScene(this);
    ++m_numLayers;
}

bool Scene::RemoveLayer(Layer* layer)
{
    int index = IndexOf(layer);
    if (index == -1)
        return false;

    RemoveLayerAt(index);
    return true;
}

Layer* Scene::RemoveLayerAt(int index)
{
    Layer* layer = GetLayerAt(index);
    for (int i = index; i < m_numLayers - 1; ++i)
    {
        m_layers[i] = m_layers[i + 1];
    }
    layer->SetScene(nullptr);
    return layer;
}

Camera& Scene::GetCamera() { return m_camera; }

void Scene::GrowIfNeed()
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
