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

b2Fixture &ContactInfo::GetCollider() const
{
    return *m_collider;
}

void ContactInfo::SetIsEnabled(bool flag) const
{
    m_contact->SetEnabled(flag);
}

bool ContactInfo::IsEnabled() const
{
    return m_contact->IsEnabled();
}

b2Fixture &ContactInfo::GetOtherCollider() const
{
    return *m_otherCollider;
}

const Identifier* ContactInfo::GetOtherIdentifier() const 
{
	return m_identifier;
}
