#ifndef MONSTER_H
#define MONSTER_H
#include "Animator.h"
#include "CollisionHandler.h"
#include "Destroyable.h"
#include "GameObject.h"
#include "Rect.h"
#include "Utils.h"
#include "box2d/b2_body.h"
#include "box2d/box2d.h"
class Level;
class Monster : public GameObject, public ICollisionHandler, public IDestroyable
{
  public:
    ~Monster();

    bool Init(const FloatRect& bounding);
    void Tick(float deltaTime) override;

    // IDestroyable interface
  public:
    bool TakeDamge(int damage, Direction direction) override;
    int  GetHitPoints() override;
    int  GetMaxHitPoints() override;
    bool IsDead() override;

    // ICollisionHandler interface
  public:
    void OnBeginContact(const ContactInfo& info) override;
    void OnEndContact(const ContactInfo& info) override;
    void OnPreSolve(const ContactInfo& info,
                    const b2Manifold&  oldManiflod) override;
    void OnPostSolve(const ContactInfo&      info,
                     const b2ContactImpulse& impluse) override;

  protected:
    Monster();
    b2Body*    m_body;
    Identifier m_identifier;
    int        m_hitPoints;
    int        m_maxHitPoints;
    int        m_damageWhenTouching;
};
#endif // MONSTER_H
