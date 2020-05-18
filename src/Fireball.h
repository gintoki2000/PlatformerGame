#ifndef FIREBALL_H
#define FIREBALL_H
#include "Animation.h"
#include "CollisionHandler.h"
#include "GameObject.h"
#include "Spell.h"
#include "Utils.h"
#include "Vec.h"
#include "box2d/box2d.h"
class Fireball : public Spell, public ICollisionHandler
{
  public:
    static Fireball* Create(const Vec2& position, Direction direction,
                            float speed);
	static Fireball* Create(const Vec2& position, Direction direction);
    ~Fireball();

  public:
    // GameObject interface
    void Tick(float deltaTime) override;
    void Paint() override;
	void Start() override;
    void Cleanup() override;

    // ICollisionHandler interface
  public:
    void OnBeginContact(const ContactInfo& info) override;
    void OnEndContact(const ContactInfo& info) override;
    void OnPostSolve(const ContactInfo&      info,
                     const b2ContactImpulse& impluse) override;
    void OnPreSolve(const ContactInfo& info,
                    const b2Manifold&  oldManiflod) override;

  private:
    Fireball();
    bool        Init(const Vec2& position, Direction direction, float speed);
    SpriteSheet m_spriteSheet;
    Animation*  m_animation;
    b2Body*     m_body;
    SDL_RendererFlip m_flip;
    float            m_timer;
    Identifier       m_identifier;
};
#endif // FIREBALL_H
