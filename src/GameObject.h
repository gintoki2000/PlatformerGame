#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "Enums.h"
#include "NTRect.h"
#include "SDL.h"
#include "box2d/b2_body.h"
class Level;
class GameObject
{

  public:
    /// constructor && destructor
    GameObject(Level* level);
    virtual ~GameObject();
    /// stuffs
    virtual void tick(float deltaTime) = 0;
    virtual void paint()               = 0;

    /// setter && getter
    float        getPositionX() const;
    virtual void setPositionX(float x);

    float        getPositionY() const;
    virtual void setPositionY(float y);

    virtual void setPosition(float x, float y);

    double       getRotation() const;
    virtual void setRotation(double rotation);

    Level* getLevel() const;
    void   setLevel(Level* level);

    bool         isVisible() const;
    virtual void setVisible(bool visible);

    bool         isAlive() const;
    virtual void setIsAlive(bool isAlive);

    virtual void onBeginContact(b2Contact* /*contact*/,
                                b2Fixture* /*otherFixture*/)
    {
    }
    virtual void onEndContact(b2Contact* /*contact*/,
                              b2Fixture* /*otherFixture*/)
    {
    }

  protected:
    friend class Animator;
    friend class Body;
    Level* m_level;
    float  m_positionX;
    float  m_positionY;
    double m_rotation;
    bool   m_isVisible;
    bool   m_isAlive;
};
#endif // GAME_OBJECT_H
