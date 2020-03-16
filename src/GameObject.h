#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "NTRect.h"
#include "SDL.h"
class Level;
class GameObject
{
  private:
    friend class ObjectLayer;
    int m_type;
    GameObject* m_next;
    GameObject* m_prev;

  protected:
    Level* m_level;
    float m_positionX;
    float m_positionY;
    float m_rotation;

  public:
    /// constructor && destructor
    GameObject(int type);
    virtual ~GameObject();

    virtual void render(float deltaTime) = 0;

    /// setter && getter
    float getPositionX() const;
    virtual void setPositionX(float x);

    float getPositionY() const;
    virtual void setPositionY(float y);

    virtual void setPosition(float x, float y);

    Level* getLevel() const { return m_level; }
    void setLevel(Level* level) { m_level = level; }

    int getGameObjectType() const { return m_type; }

  protected:
    virtual void onPositionChanged();
};
#endif // GAME_OBJECT_H
