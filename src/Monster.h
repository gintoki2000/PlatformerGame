#ifndef MONSTER_H
#define MONSTER_H
#include "Animator.h"
#include "CollisionHandler.h"
#include "Destroyable.h"
#include "GameObject.h"
#include "Rect.h"
#include "box2d/b2_body.h"
#include "box2d/box2d.h"
class Level;
class Monster : public GameObject, public ICollisionHandler, public IDestroyable
{
  public:
    ~Monster();

    virtual bool init(const Rect& bounding);
    virtual void tick(float deltaTime) override;

    // IDestroyable interface
  public:
    virtual void takeDamge(int damage) override;
    virtual int  getHitPoints() override;
    virtual int  getMaxHitPoints() override;
    virtual bool isDead() override;

	// ICollisionHandler interface
  public:
    void onBeginContact(const ContactInfo& info) override;
    void onEndContact(const ContactInfo& info) override;
    void onPreSolve(const ContactInfo& info,
                    const b2Manifold&  oldManiflod) override;
    void onPostSolve(const ContactInfo&      info,
                     const b2ContactImpulse& impluse) override;

  protected:
    Monster();
    b2Body* m_body;
    int     m_hitPoints;
    int     m_maxHitPoints;
};
#endif // MONSTER_H
