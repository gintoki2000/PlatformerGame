#include "CollisionHandler.h"
#include "Utils.h"

ICollisionHandler::ICollisionHandler()
{

}

ICollisionHandler::~ICollisionHandler()
{

}


ContactInfo::ContactInfo(b2Contact *contact, b2Fixture *collider, b2Fixture *otherCollider, Identifier* indentifier):
    m_contact(contact), m_collider(collider), m_otherCollider(otherCollider), m_identifier(indentifier)
{

}

b2Fixture &ContactInfo::getCollider() const
{
    return *m_collider;
}

void ContactInfo::setIsEnabled(bool flag) const
{
    m_contact->SetEnabled(flag);
}

bool ContactInfo::isEnabled() const
{
    return m_contact->IsEnabled();
}

b2Fixture &ContactInfo::getOtherCollider() const
{
    return *m_otherCollider;
}

const Identifier* ContactInfo::getOtherIdentifier() const 
{
	return m_identifier;
}
