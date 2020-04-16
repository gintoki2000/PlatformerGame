#ifndef COLLISIONCALLBACK_H
#define COLLISIONCALLBACK_H
#include "Utils.h"
#include "box2d/box2d.h"

class ContactInfo
{
public:
    ContactInfo(b2Contact* contact, b2Fixture* collider, b2Fixture* otherCollider, Identifier*  indentifier);

    b2Fixture &getCollider() const;
    b2Fixture &getOtherCollider() const;
    void setIsEnabled(bool flag) const;
    bool isEnabled() const;
	const Identifier* getOtherIdentifier() const;


private:
    b2Contact* m_contact;
    b2Fixture* m_collider;
    b2Fixture* m_otherCollider;
	Identifier* m_identifier;
};

class ICollisionCallback
{
public:
    ICollisionCallback();
    virtual ~ICollisionCallback();

    virtual void onPreSolve(const ContactInfo& info, const b2Manifold& oldManiflod) {};
    virtual void onBeginContact(const ContactInfo& info) {};
    virtual void onEndContact(const ContactInfo& info) {};
    virtual void onPostSolve(const ContactInfo& info, const b2ContactImpulse& impluse) {};
};

#endif // COLLISIONCALLBACK_H
