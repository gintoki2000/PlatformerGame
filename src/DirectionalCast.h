#ifndef DIRECTIONAL_CAST_H
#define DIRECTIONAL_CAST_H
#include "Adventurer.h"
#include "Animator.h"
#include "Input.h"
#include "Level.h"
#include "ObjectLayer.h"
#include "Projectile.h"
#include "Technique.h"
#include "Utils.h"
#include "Vec.h"
template <class TProjectile> class DirectionalCast : public Technique
{
  public:
    DirectionalCast(int mpCost, const Sprite& icon) : Technique(mpCost, icon) {}

    void Enter(Adventurer& adventurer) override
    {
        adventurer.GetAnimator()->PushState(Adventurer::ANIM_CAST_SPELL);
        if (adventurer.IsGrounded())
        {
            adventurer.m_horizontalAcceleration = 0.f;
            m_isInAir                           = false;
        }

        m_phrase = 0;
        m_timer  = 0.f;
    }

    bool Tick(Adventurer& adventurer, float deltaTime) override
    {
        m_timer += deltaTime;
        switch (m_phrase)
        {
        case 0:
        {
            if (adventurer.GetAnimator()->IsCurrentAnimationFinshed())
            {
                TProjectile* spell = TProjectile::Create(adventurer.GetPosition(),
                                               adventurer.GetDirection());
                SDL_assert(spell != nullptr);
                Level* level = static_cast<Level*>(adventurer.GetScene());
                level->GetSpriteLayer()->AddObject(spell);

                adventurer.GetAnimator()->Play(Adventurer::ANIM_CAST_LOOP);
                m_timer  = 0.f;
                m_phrase = 1;
            }
        }
        break;
        case 1:
        {
            if (m_timer >= 0.1f)
            {
                return true;
            }
        }
        break;
        }
        if (m_isInAir)
        {
            if (!adventurer.IsGrounded())
            {
                int inputDirection = Input::GetHorizontalInputDirection();
                adventurer.m_horizontalAcceleration =
                    inputDirection * adventurer.m_runAcceleration;
            }
            else
            {
                // just grounded
                if (!adventurer.WasGrounded())
                {
                    adventurer.m_horizontalAcceleration = 0.f;
                }
            }
        }
        return false;
    }

    void Exit(Adventurer& adventurer) override
    {
        adventurer.GetAnimator()->PopState();
    }

  private:
    int   m_phrase;
    float m_timer;
    bool  m_isInAir;
};
#endif // DIRECTIONAL_CAST_H
