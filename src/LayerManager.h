#ifndef LAYER_MANAGER_H
#define LAYER_MANAGER_H
#include "Layer.h"
#include "Camera.h"
class LayerManager
{
  public:
    LayerManager();
    virtual ~LayerManager();

    virtual void update(float deltaTime);
    virtual void render();

    int    getNumLayers() const;
    Layer* getLayerAt(int index) const;
    Layer* getLayerByName(const char* layerName);
    int    indexOf(Layer* layer) const;
    void   addLayer(Layer* layer);
    void   addLayer(Layer* layer, int index);
    bool   removeLayer(Layer* layer);
    Layer* removeLayerAt(int index);
    Camera& getCamera();

  private:
    void    growIfNeed();
    Layer** m_layers;
    int     m_capacity;
    int     m_numLayers;
    Camera  m_camera;
};
#endif // LAYER_MANAGER_H
