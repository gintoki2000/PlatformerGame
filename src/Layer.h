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

    virtual void Tick(float deltaTime) = 0;
    virtual void Paint()                = 0;
	virtual void Start();

    bool IsVisible() const { return m_isVisible; }
    void SetIsVisible(bool isVisible);
    void Show();
    void Hide();
    bool IsActive() const { return m_isActive; }
    void SetIsActive(bool isActive);
    void Activate() { SetIsActive(true); }
    void Deactivate() { SetIsActive(false); }

    Scene*             GetScene() const;
    void               SetScene(Scene* manager);
    const std::string& GetName() const;
    void               SetName(const std::string& name);

  protected:
    virtual void OnBecomeVisible();
    virtual void OnBecomeInvisible();

  private:
    bool        m_isVisible;
    bool        m_isActive;
    Scene*      m_manager;
    std::string m_name;
};
#endif // LAYER_H
