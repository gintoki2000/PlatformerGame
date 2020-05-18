#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "Vec.h"
class ObjectLayer;
class Scene;
class GameObject
{

  public:
    /// constructor && destructor
    GameObject();
    virtual ~GameObject();
    /// stuffs
    virtual void Tick(float deltaTime);
    virtual void Paint(); 
    virtual void Cleanup();
    virtual void Start();
    void         Show();
    void         Hide();

    /// setter && getter
    float GetPositionX() const;
    void  SetPositionX(float x);

    float GetPositionY() const;
    void  SetPositionY(float y);

    Vec2 GetPosition() const;
    void SetPosition(float x, float y);
    void SetPosition(const Vec2& pos);

    double GetRotation() const;
    void   SetRotation(double rotation);

    void Remove();

    bool IsVisible() const;
    void SetIsVisible(bool isVisible);

    bool IsActive() const;
    void SetIsActive(bool isActive);

    ObjectLayer* GetObjectLayer() const;
    void         SetObjectLayer(ObjectLayer* objectLayer);

    Scene* GetScene() const;

  protected:
    virtual void OnPositionChanged();
    virtual void OnRotationChanged();
    virtual void OnAcivated();
    virtual void OnDeactivated();
    virtual void OnBecomeVisible();
    virtual void OnBecomeInvisible();

    ObjectLayer*  m_objectLayer;
    float         m_positionX;
    float         m_positionY;
    double        m_rotation;
    bool          m_isVisible;
    bool          m_isActive;
    bool          m_needToRemove;

    friend class ObjectLayer;
};
#endif // GAME_OBJECT_H
