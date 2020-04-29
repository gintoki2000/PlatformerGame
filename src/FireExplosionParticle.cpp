#include "FireExplosionParticle.h"

void FireExplosionParticle::cleanup() { getPool()->free(this); }

bool FireExplosionParticle::preinit() { 
	if (!AnimatedParticle::preinit(TextureManager::FIRE_EXPLOSION, 0.02f, 96, 96))
	{
		return false;
	}
	return true;
}
