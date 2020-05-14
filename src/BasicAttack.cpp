#include "BasicAttack.h"
#include "Adventurer.h"
#include "Animator.h"
#include "Audio.h"
#include "Input.h"
#include "Monster.h"
#include "Sprite.h"
#include "Technique.h"
#include "TextureManager.h"
#include "Utils.h"
#include "Vec.h"

BasicAttack::BasicAttack() : Technique(0, makeSprite(TEX_ICONS, {16, 0, 16, 16}))
{
}

bool BasicAttack::tick(Adventurer& adventurer, float)
{
    switch (m_phrase)
    {
    case 0:
    {
        if (Input::isJustPressed(BUTTON_A))
        {
            m_chain = true;
        }
        if (adventurer.getAnimator()->isCurrentAnimationFinshed())
        {
            check(adventurer.getPosition(), {20.f, 30.f},
                  adventurer.getDirection());
            if (m_chain)
            {
                m_phrase = 1;
                m_chain  = false;
                adventurer.getAnimator()->play(Adventurer::ANIM_ATK_2);
            }
            else
            {
                return true;
            }
        }
    }
    break;
    case 1:
    {
        if (Input::isJustPressed(BUTTON_A))
        {
            m_chain = true;
        }
        if (adventurer.getAnimator()->isCurrentAnimationFinshed())
        {
            check(adventurer.getPosition(), {35.f, 20.f},
                  adventurer.getDirection());
            if (m_chain)
            {
                m_phrase = 2;
                m_chain  = false;
                adventurer.getAnimator()->play(Adventurer::ANIM_ATK_3);
            }
            else
            {
                return true;
            }
        }
    }
    break;
    case 2:
    {
        if (adventurer.getAnimator()->isCurrentAnimationFinshed())
        {
            check(adventurer.getPosition(), {20.f, 30.f},
                  adventurer.getDirection());
            return true;
        }
    }
    }
    return false;
}

void BasicAttack::enter(Adventurer& adventurer)
{
    adventurer.getAnimator()->pushState(Adventurer::ANIM_ATK_1);
    adventurer.m_horizontalAcceleration = 0.f;
    m_chain                             = false;
    m_phrase                            = 0;
    b2Vec2 vel = adventurer.getBody()->GetLinearVelocity();
    vel.x      = 0.f;
    adventurer.getBody()->SetLinearVelocity(vel);
}

void BasicAttack::exit(Adventurer& adventurer)
{
    adventurer.getAnimator()->popState();
}

void BasicAttack::check(const Vec2& position, const Vec2& size,
                        Direction direction)
{

    const int  MAX = 10;
    b2Fixture* fixtures[MAX];
    int        n = 0;
    FloatRect  rect;

    rect.x = position.x + directionToSign(direction) * size.x / 2.f;
    rect.y = position.y;

    rect.width  = size.x;
    rect.height = size.y;
    boxQuery(rect, CATEGORY_BIT_MONSTER, fixtures, n, MAX);

    for (int i = 0; i < n; ++i)
    {
        const Identifier* idr = static_cast<const Identifier*>(
            fixtures[i]->GetBody()->GetUserData());
        Monster* monster = static_cast<Monster*>(idr->object);
        if (monster->takeDamge(5, DIRECTION_NONE))
        {
            Audio::play(SOUND_STAB);
        }
    }
}
