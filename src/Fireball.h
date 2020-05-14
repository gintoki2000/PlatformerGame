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
    static Fireball* create(const Vec2& position, Direction direction,
                            float speed);
	static Fireball* create(const Vec2& position, Direction direction);
    ~Fireball();

  public:
    // GameObject interface
    void tick(float deltaTime) override;
    void paint() override;
	void start() override;
    void cleanup() override;

    // ICollisionHandler interface
  public:
    void onBeginContact(const ContactInfo& info) override;
    void onEndContact(const ContactInfo& info) override;
    void onPostSolve(const ContactInfo&      info,
                     const b2ContactImpulse& impluse) override;
    void onPreSolve(const ContactInfo& info,
                    const b2Manifold&  oldManiflod) override;

  private:
    Fireball();
    bool        init(const Vec2& position, Direction direction, float speed);
    SpriteSheet m_spriteSheet;
    Animation*  m_animation;
    b2Body*     m_body;
    SDL_RendererFlip m_flip;
    float            m_timer;
    Identifier       m_identifier;
};
#endif // FIREBALL_H