#ifndef AXE_H
#define AXE_H
#include "CollisionHandler.h"
#include "SDL.h"
#include "Projectile.h"
#include "SpriteSheet.h"
#include "Utils.h"
class Axe : public Projectile, public ICollisionHandler
{
  public:
    static Axe* Create(const Vec2& adventurerPos, Direction adventurerDir);
    ~Axe();

    /// GameObject interface
  public:
    void Tick(float deltaTime) override;
    void Paint() override;
    void Cleanup() override;

    /// ICollisionHandler interface
  public:
    void OnBeginContact(const ContactInfo& info) override;
    void OnEndContact(const ContactInfo& info) override;
    void OnPreSolve(const ContactInfo& info,
                    const b2Manifold&  oldManiflod) override;
    void OnPostSolve(const ContactInfo&      info,
                     const b2ContactImpulse& impluse) override;

  private:
    Axe();
    bool Init(const Vec2& pos, Direction dir);

    Sprite           m_sprite;
    SDL_RendererFlip m_flip;
    b2Body*          m_body;
    Identifier       m_identifier;
    int              m_frameCounter;

    static constexpr int NUM_SHADOWS = 4;
    struct Shadow
    {
        float x;
        float y;
        float r;
    } m_shadows[NUM_SHADOWS];
};
#endif // AXE_H
