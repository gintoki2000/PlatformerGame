#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "Vec.h"
class ObjectLayer;
class LayerManager;
class GameObject
{

  public:
    /// constructor && destructor
    GameObject();
    virtual ~GameObject();
    /// stuffs
    virtual void tick(float deltaTime) = 0;
    virtual void paint()               = 0;
    virtual void cleanup();
    virtual void start();
    void         show();
    void         hide();

    /// setter && getter
    float getPositionX() const;
    void  setPositionX(float x);

    float getPositionY() const;
    void  setPositionY(float y);

    Vec2 getPosition() const;
    void setPosition(float x, float y);
    void setPosition(const Vec2& pos);

    double getRotation() const;
    void   setRotation(double rotation);

    void remove();

    bool isVisible() const;
    void setIsVisible(bool isVisible);

    bool isActive() const;
    void setIsActive(bool isActive);

    ObjectLayer* getObjectLayer() const;
    void         setObjectLayer(ObjectLayer* objectLayer);

    LayerManager* getLayerManager() const;

  protected:
    virtual void onPositionChanged();
    virtual void onRotationChanged();
    virtual void onAcivated();
    virtual void onDeactivated();
    virtual void onBecomeVisible();
    virtual void onBecomeInvisible();

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
