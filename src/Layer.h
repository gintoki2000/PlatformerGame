#ifndef LAYER_H
#define LAYER_H
#include "Rect.h"
#include <string>
class Scene;
class Layer
{
  public:
    Layer();

    virtual ~Layer() = 0;

    virtual void update(float deltaTime) = 0;
    virtual void render()                = 0;
	virtual void start();

    bool isVisible() const { return m_isVisible; }
    void setIsVisible(bool isVisible);
    void show();
    void hide();
    bool isActive() const { return m_isActive; }
    void setIsActive(bool isActive);
    void activate() { setIsActive(true); }
    void deactivate() { setIsActive(false); }

    Scene*             getScene() const;
    void               setScene(Scene* manager);
    const std::string& getName() const;
    void               setName(const std::string& name);

  protected:
    virtual void onBecomeVisible();
    virtual void onBecomeInvisible();

  private:
    bool        m_isVisible;
    bool        m_isActive;
    Scene*      m_manager;
    std::string m_name;
};
#endif // LAYER_H
