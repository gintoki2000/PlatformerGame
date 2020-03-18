#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "NTRect.h"
#include "SDL.h"
class Level;
class GameObject
{

  public:
    /// constructor && destructor
    GameObject(int gameObjectType);
    virtual ~GameObject();

    virtual void render(float deltaTime) = 0;

    /// setter && getter
    int  getPositionX() const;
    void setPositionX(int x);

    int  getPositionY() const;
    void setPositionY(int y);

    void setPosition(int x, int y);

    Level* getLevel() const;
    void   setLevel(Level* level);

    bool         isVisible() const;
    virtual void setVisible(bool visible);

    int getGameObjectType() const { return m_gameObjectType; }

  protected:
    virtual void onPositionChanged();

    virtual void onSizeChanged();

    Level* m_level;
    int    m_positionX;
    int    m_positionY;
    double m_rotation;
    bool   m_isVisible;

  private:
    friend class ObjectLayer;
    int         m_gameObjectType;
    GameObject* m_next;
    GameObject* m_prev;
};
#endif // GAME_OBJECT_H
