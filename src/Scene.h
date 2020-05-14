#ifndef LAYER_MANAGER_H
#define LAYER_MANAGER_H
#include "Camera.h"
#include "Layer.h"
class Scene
{
  public:
    Scene();
    virtual ~Scene();

    virtual void update(float deltaTime);
    virtual void render();
    virtual void start();

    int     getNumLayers() const;
    Layer*  getLayerAt(int index) const;
    Layer*  getLayerByName(const char* layerName);
    int     indexOf(Layer* layer) const;
    void    addLayer(Layer* layer);
    void    addLayer(Layer* layer, int index);
    bool    removeLayer(Layer* layer);
    Layer*  removeLayerAt(int index);
    Camera& getCamera();

  private:
    void    growIfNeed();
    Layer** m_layers;
    int     m_capacity;
    int     m_numLayers;
    Camera  m_camera;
    bool    m_isStarted;
};
#endif // LAYER_MANAGER_H
