#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "NTRect.h"
#include "SDL.h"
class Level;
class GameObject
{

  public:
    /// constructor && destructor
	GameObject(int gameObjectType, Level* level);
    virtual ~GameObject();
    /// stuffs
    virtual void tick(float deltaTime) = 0;
    virtual void paint() = 0;

    /// setter && getter
    float getPositionX() const;
    void  setPositionX(float x);

    float getPositionY() const;
    void  setPositionY(float y);

    void setPosition(float x, float y);

    Level* getLevel() const;
    void   setLevel(Level* level);

    bool         isVisible() const;
    virtual void setVisible(bool visible);

    int getGameObjectType() const { return m_gameObjectType; }

  protected:
    virtual void onPositionChanged();

    Level* m_level;
    float  m_positionX;
    float  m_positionY;
    double m_rotation;
    bool   m_isVisible;

  private:
    friend class ObjectList;
    int         m_gameObjectType;
    GameObject* m_next;
    GameObject* m_prev;
};
#endif // GAME_OBJECT_H
