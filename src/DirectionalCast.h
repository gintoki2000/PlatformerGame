#ifndef DIRECTIONAL_CAST_H
#define DIRECTIONAL_CAST_H
#include "Animator.h"
#include "Input.h"
#include "Level.h"
#include "ObjectLayer.h"
#include "Player.h"
#include "PlayerSkill.h"
#include "Spell.h"
#include "Utils.h"
#include "Vec.h"
template <class TSpell> class DirectionalCast : public PlayerSkill
{
  public:
    static DirectionalCast* create() { return new DirectionalCast; }
    ~DirectionalCast() { DELETE_NULL(m_spell); }

    bool activate(Player& player) override
    {
        if (Input::isButtonAJustPressed() && Input::isButtonUpPressed())
        {
            Level* level;
            m_spell = TSpell::create(player.getPosition(), player.m_direction);

            if (player.consumeMana(m_spell->getManaCost()))
            {
                player.getAnimator()->pushState(Player::ANIM_CAST_SPELL);
                if (player.isGrounded())
                {
                    player.m_horiziontalAcceleration = 0.f;
                    m_isInAir                        = true;
                }

                level = static_cast<Level*>(player.getLayerManager());
                level->getSpriteLayer()->addObject(m_spell);
                m_spell  = nullptr;
                m_phrase = 0;
                m_timer  = 0.f;

                return true;
            }
            else
            {
                DELETE_NULL(m_spell);
            }
        }
        return false;
    }
    bool tick(Player& player, float deltaTime) override
    {
        m_timer += deltaTime;
        switch (m_phrase)
        {
        case 0:
        {
            if (player.getAnimator()->isCurrentAnimationFinshed())
            {
                player.getAnimator()->play(Player::ANIM_CAST_LOOP);
                m_timer  = 0.f;
                m_phrase = 1;
            }
        }
        break;
        case 1:
        {
            if (m_timer >= 0.1f)
            {
                player.getAnimator()->popState();
                Level* level;
                return true;
            }
        }
        break;
        }
        if (m_isInAir)
        {
            if (!player.isGrounded())
            {
                int inputDirection = Input::getHorizontalInputDirection();
                player.m_horiziontalAcceleration =
                    inputDirection * player.m_runAcceleration;
            }
            else
            {
                // just grounded
                if (!player.wasGrounded())
                {
                    player.m_horiziontalAcceleration = 0.f;
                }
            }
        }
        return false;
    }

  private:
    DirectionalCast() : m_spell(nullptr) {}

    int    m_phrase;
    float  m_timer;
    bool   m_isInAir;
    Spell* m_spell;
};
#endif // DIRECTIONAL_CAST_H
