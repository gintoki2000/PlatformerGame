#ifndef GRENADE_H
#define GRENADE_H
#include "Animation.h"
#include "CollisionHandler.h"
#include "Spell.h"
#include "SpriteSheet.h"
#include "Utils.h"
#include "Vec.h"
#include "box2d/box2d.h"
class Grenade : public Spell, public ICollisionHandler
{
  public:
    static Grenade* create(const Vec2& pos, Direction dir);
    ~Grenade();

    /// GameObject interface
  public:
    void tick(float deltaTime) override;
    void paint() override;
    void cleanup() override;

    /// ICollisionHandler interface
  public:
    void onPreSolve(const ContactInfo& info,
                    const b2Manifold&  oldManiflod) override;
    void onBeginContact(const ContactInfo& info) override;
    void onEndContact(const ContactInfo& info) override;
    void onPostSolve(const ContactInfo&      info,
                     const b2ContactImpulse& impluse) override;

  private:
    Grenade();
    bool init(const Vec2& pos, Direction dir);

    Sprite           m_sprite;
    b2Body*          m_body;
    float            m_timer;
    SDL_RendererFlip m_flip;
    Identifier       m_identifier;

    static constexpr int SPRITE_WIDTH  = 12;
    static constexpr int SPRITE_HEIGHT = 12;
};
#endif // GRENADE_H
