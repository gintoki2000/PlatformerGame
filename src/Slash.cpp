#include "Slash.h"
#include "Animator.h"
#include "AssertManager.h"
#include "BloodStainParticle.h"
#include "Game.h"
#include "Input.h"
#include "Level.h"
#include "Monster.h"
#include "ParticleSystem.h"
#include "Player.h"
#include "Rect.h"
#include "SDL_mixer.h"
#include "Utils.h"

Slash* Slash::create()
{
    Slash* ret = new Slash;

    if (ret->init())
    {
        return ret;
    }
    delete ret;
    return nullptr;
}

bool Slash::activate(Player& player)
{
    if (Input::isButtonAJustPressed())
    {
        return true;
    }
    return false;
}

void Slash::enter(Player& player)
{
    player.getAnimator()->pushState(Player::ANIM_ATK_1);
    player.m_horiziontalAcceleration = 0.f;
}

bool Slash::tick(Player& player, float deltaTime)
{
    if (player.getAnimator()->isCurrentAnimationFinshed())
    {

        const int  MAX = 10;
        b2Fixture* fixtures[MAX];
        int        n = 0;
        FloatRect  rect(player.getPosition(), Vec2(50.f, 50.f));
        boxQuery(rect, CATEGORY_BIT_MONSTER, fixtures, n, MAX);

		Level* level = static_cast<Level*>(player.getLayerManager());
        for (int i = 0; i < n; ++i)
        {
            void*             userData = fixtures[i]->GetBody()->GetUserData();
            const Identifier* idr = static_cast<const Identifier*>(userData);
            Monster*          monster = static_cast<Monster*>(idr->object);
            monster->takeDamge(2, DIRECTION_NONE);

			level->getCameraShaker()->shake(0.1f, 5, 20);			
			level->getParticleSystem()->create<BloodStainParticle>(player.getPosition());

        }
        return true;
    }
    return false;
}

void Slash::exit(Player& player) { player.getAnimator()->popState(); }

Slash::Slash() {}

bool Slash::init() { return true; }
