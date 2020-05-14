#ifndef AXE_H
#define AXE_H
#include "CollisionHandler.h"
#include "SDL.h"
#include "Spell.h"
#include "SpriteSheet.h"
#include "Utils.h"
class Axe : public Spell, public ICollisionHandler
{
  public:
    static Axe* create(const Vec2& adventurerPos, Direction adventurerDir);
    ~Axe();

    /// GameObject interface
  public:
    void tick(float deltaTime) override;
    void paint() override;
    void cleanup() override;

    /// ICollisionHandler interface
  public:
    void onBeginContact(const ContactInfo& info) override;
    void onEndContact(const ContactInfo& info) override;
    void onPreSolve(const ContactInfo& info,
                    const b2Manifold&  oldManiflod) override;
    void onPostSolve(const ContactInfo&      info,
                     const b2ContactImpulse& impluse) override;

  private:
    Axe();
    bool init(const Vec2& pos, Direction dir);

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
