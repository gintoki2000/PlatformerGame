#include "FireBustParticle.h"
#include "Animation.h"
#include "AssertManager.h"
#include "Game.h"
#include "LayerManager.h"
#include "SDL_render.h"

void FireBustParticle::cleanup() { getPool()->free(this); }

bool FireBustParticle::preinit()
{
    if (!AnimatedParticle::preinit(TextureManager::FIRE_BUST, 0.05f,
                                   SPRITE_WIDTH, SPRITE_HEIGHT))
    {
		return false;
    }
	return true;
}
