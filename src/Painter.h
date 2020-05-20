#ifndef PAINTER_H
#define PAINTER_H
#include "Animation.h"
#include "Animator.h"
#include "GameObject.h"
#include "Sprite.h"
class Painter
{

  public:
    SDL_RendererFlip GetFlip() const { return m_flip; }
    void             SetFlip(SDL_RendererFlip flip) { m_flip = flip; }
    SDL_Point        GetAnchor() const { return m_anchor; }
    void             SetAnchor(const SDL_Point& anchor) { m_anchor = anchor; }

  protected:
    SDL_RendererFlip m_flip;
    SDL_Point        m_anchor;
};

class SpritePainter
{
  public:
    SpritePainter(const Sprite* sprite);

    void Paint(GameObject& obj);

    const Sprite* GetSprite() const { return m_sprite; }
    void          SetSprite(const Sprite* sprite) { m_sprite = sprite; }

  private:
    const Sprite* m_sprite;
};

class AnimationPainter : public Painter
{
  private:
    const Animation* m_animation;
};

class AnimatorPainter : public Painter
{
  private:
    const Animator* m_animator;
};
#endif // PAINTER_H
