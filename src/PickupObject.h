#ifndef PICKUP_OBJECT_H
#define PICKUP_OBJECT_H
#include "CollisionHandler.h"
#include "GameObject.h"
#include "SpriteSheet.h"
#include "Utils.h"
#include "box2d/b2_body.h"

class PickupObject : public GameObject, ICollisionHandler
{
  public:
    PickupObject(const Sprite& sprite, const Vec2& position);

	~PickupObject();

    // GameObject interface
  public:
	void Tick(float deltaTime) override;
    void Paint() override;
    void Start() override;
	void Cleanup() override;

    // ICollisionHandler interface
  public:
    void OnPreSolve(const ContactInfo& info,
                    const b2Manifold&  oldManiflod) override;
    void OnBeginContact(const ContactInfo& info) override;
    void OnEndContact(const ContactInfo& info) override;
    void OnPostSolve(const ContactInfo&      info,
                     const b2ContactImpulse& impluse) override;

  protected:
    virtual void OnCollected();

  private:
    Sprite     m_sprite;
    b2Body*    m_body;
    Identifier m_identifier;
};
#endif // PICKUP_OBJECT_H
