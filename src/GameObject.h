#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
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

    /// setter && getter
    float        getPositionX() const;
    virtual void setPositionX(float x);

    float        getPositionY() const;
    virtual void setPositionY(float y);

    virtual void setPosition(float x, float y);

    double       getRotation() const;
    virtual void setRotation(double rotation);

    bool needToRemove() const;
    void scheduleRemove();

    bool isVisible() const;
    void setIsVisible(bool isVisible);

    bool isActive() const;
    void setIsActive(bool isActive);

    void setNeedToRemove(bool needToRemove);

    ObjectLayer* getObjectLayer() const;
    void         setObjectLayer(ObjectLayer* objectLayer);

    LayerManager* getLayerManager() const;
    void          setLayerManager(LayerManager* layerManager);

  protected:
    friend class Animator;
    ObjectLayer*  m_objectLayer;
    LayerManager* m_layerManager;
    float         m_positionX;
    float         m_positionY;
    double        m_rotation;
    bool          m_isVisible;
    bool          m_isActive;
    bool          m_needToRemove;
};
#endif // GAME_OBJECT_H
