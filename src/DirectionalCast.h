#ifndef DIRECTIONAL_CAST_H
#define DIRECTIONAL_CAST_H
#include "Adventurer.h"
#include "Animator.h"
#include "Input.h"
#include "Level.h"
#include "ObjectLayer.h"
#include "Spell.h"
#include "Technique.h"
#include "Utils.h"
#include "Vec.h"
template <class TSpell> class DirectionalCast : public Technique
{
  public:
    DirectionalCast(int mpCost, const Sprite& icon) : Technique(mpCost, icon) {}

    void enter(Adventurer& adventurer) override
    {
        adventurer.getAnimator()->pushState(Adventurer::ANIM_CAST_SPELL);
        if (adventurer.isGrounded())
        {
            adventurer.m_horizontalAcceleration = 0.f;
            m_isInAir                           = false;
        }

        m_phrase = 0;
        m_timer  = 0.f;
    }

    bool tick(Adventurer& adventurer, float deltaTime) override
    {
        m_timer += deltaTime;
        switch (m_phrase)
        {
        case 0:
        {
            if (adventurer.getAnimator()->isCurrentAnimationFinshed())
            {
                adventurer.getAnimator()->play(Adventurer::ANIM_CAST_LOOP);
                m_timer  = 0.f;
                m_phrase = 1;
            }
        }
        break;
        case 1:
        {
            if (m_timer >= 0.1f)
            {
                TSpell* spell = TSpell::create(adventurer.getPosition(),
                                               adventurer.getDirection());
                SDL_assert(spell != nullptr);
                Level* level = static_cast<Level*>(adventurer.getScene());
                level->getSpriteLayer()->addObject(spell);
                return true;
            }
        }
        break;
        }
        if (m_isInAir)
        {
            if (!adventurer.isGrounded())
            {
                int inputDirection = Input::getHorizontalInputDirection();
                adventurer.m_horizontalAcceleration =
                    inputDirection * adventurer.m_runAcceleration;
            }
            else
            {
                // just grounded
                if (!adventurer.wasGrounded())
                {
                    adventurer.m_horizontalAcceleration = 0.f;
                }
            }
        }
        return false;
    }

    void exit(Adventurer& adventurer) override
    {
        adventurer.getAnimator()->popState();
    }

  private:
    int   m_phrase;
    float m_timer;
    bool  m_isInAir;
};
#endif // DIRECTIONAL_CAST_H
