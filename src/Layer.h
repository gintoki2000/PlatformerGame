#ifndef LAYER_H
#define LAYER_H
#include "Rect.h"
#include <string>
class LayerManager;
class Layer
{
  public:
    Layer();

    virtual ~Layer() = 0;

    virtual void update(float deltaTime) = 0;
    virtual void render()                = 0;

    bool isVisible() const { return m_isVisible; }
    void setIsVisible(bool isVisible);
    void show() { setIsVisible(true); }
    void hide() { setIsVisible(false); }
    bool isActive() const { return m_isActive; }
    void setIsActive(bool isActive);
    void activate() { setIsActive(true); }
    void deactivate() { setIsActive(false); }

    LayerManager* getManager() const;
    void          setManager(LayerManager* manager);

    std::string getName() const;
    void        setName(const std::string& name);

  private:
    bool          m_isVisible;
    bool          m_isActive;
    LayerManager* m_manager;
    std::string   m_name;
};
#endif // LAYER_H
