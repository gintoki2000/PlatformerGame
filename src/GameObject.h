#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "NTRect.h"
#include "SDL.h"
class GameObject
{
  public:
    enum Type
    {
        PLAYER,
        MONSTER
    };

  protected:
    GameObject(Type type) : m_next(nullptr), m_prev(nullptr), m_type(type) {}

  public:
    virtual ~GameObject() {}

    GameObject* getNext() const { return m_next; }

    void setNext(GameObject* gameObject) { m_next = gameObject; }

    GameObject* getPrev() const { return m_prev; }

    void setPrev(GameObject* gameObject) { m_prev = gameObject; }

    virtual void update(float dt) = 0;

    virtual void draw(SDL_Renderer* renderer, const NTRect& viewPort) = 0;

    Type getObjectType() const { return m_type; }

    virtual void cleanup() {}

  private:
    GameObject* m_next;
    GameObject* m_prev;

    Type m_type;
};
#endif // GAME_OBJECT_H
