#ifndef LAYER_MANAGER_H
#define LAYER_MANAGER_H
#include "Camera.h"
#include "Layer.h"
class Scene
{
  public:
    Scene();
    virtual ~Scene();

    virtual void Update(float deltaTime);
    virtual void Render();
    virtual void Start();

    int     GetNumLayers() const;
    Layer*  GetLayerAt(int index) const;
    Layer*  GetLayerByName(const char* layerName);
    int     IndexOf(Layer* layer) const;
    void    AddLayer(Layer* layer);
    void    AddLayer(Layer* layer, int index);
    bool    RemoveLayer(Layer* layer);
    Layer*  RemoveLayerAt(int index);
    Camera& GetCamera();

  private:
    void    GrowIfNeed();
    Layer** m_layers;
    int     m_capacity;
    int     m_numLayers;
    Camera  m_camera;
    bool    m_isStarted;
};
#endif // LAYER_MANAGER_H
