#ifndef COLLISION_HANDLER_H
#define COLLISION_HANDLER_H
#include "Utils.h"
#include "box2d/box2d.h"

class ContactInfo
{
  public:
    ContactInfo(b2Contact* contact, b2Fixture* collider,
                b2Fixture* otherCollider, Identifier* indentifier);

    b2Fixture&        GetCollider() const;
    b2Fixture&        GetOtherCollider() const;
    void              SetIsEnabled(bool flag) const;
    bool              IsEnabled() const;
    const Identifier* GetOtherIdentifier() const;

  private:
    b2Contact*  m_contact;
    b2Fixture*  m_collider;
    b2Fixture*  m_otherCollider;
    Identifier* m_identifier;
};

class ICollisionHandler
{
  public:
    ICollisionHandler();
    virtual ~ICollisionHandler();

    virtual void OnPreSolve(const ContactInfo& info,
                            const b2Manifold&  oldManiflod)    = 0;
    virtual void OnBeginContact(const ContactInfo& info)      = 0;
    virtual void OnEndContact(const ContactInfo& info)        = 0;
    virtual void OnPostSolve(const ContactInfo&      info,
                             const b2ContactImpulse& impluse) = 0;
};

#endif // COLLISION_HANDLER_H
