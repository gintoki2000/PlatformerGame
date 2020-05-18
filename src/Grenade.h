#ifndef GRENADE_H
#define GRENADE_H
#include "Animation.h"
#include "CollisionHandler.h"
#include "Projectile.h"
#include "SpriteSheet.h"
#include "Utils.h"
#include "Vec.h"
#include "box2d/box2d.h"
class Grenade : public Projectile, public ICollisionHandler
{
  public:
    static Grenade* Create(const Vec2& pos, Direction dir);
    ~Grenade();

    /// GameObject interface
  public:
    void Tick(float deltaTime) override;
    void Paint() override;
    void Cleanup() override;

    /// ICollisionHandler interface
  public:
    void OnPreSolve(const ContactInfo& info,
                    const b2Manifold&  oldManiflod) override;
    void OnBeginContact(const ContactInfo& info) override;
    void OnEndContact(const ContactInfo& info) override;
    void OnPostSolve(const ContactInfo&      info,
                     const b2ContactImpulse& impluse) override;

  private:
    Grenade();
    bool Init(const Vec2& pos, Direction dir);

    Sprite           m_sprite;
    b2Body*          m_body;
    float            m_timer;
    SDL_RendererFlip m_flip;
    Identifier       m_identifier;

    static constexpr int SPRITE_WIDTH  = 12;
    static constexpr int SPRITE_HEIGHT = 12;
};
#endif // GRENADE_H
