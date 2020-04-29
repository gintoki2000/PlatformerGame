#ifndef FIRE_EXPLOSION_PRATICLE_H
#define FIRE_EXPLOSION_PRATICLE_H
#include "AnimatedParticle.h"
#include "Pool.h"
class FireExplosionParticle : public AnimatedParticle,
                              public PoolableObject<FireExplosionParticle>
{
	// GameObject interface
	public:
		void cleanup() override;
	private:
		bool preinit();

		friend class AnimatedParticlePool<FireExplosionParticle>;
};
#endif // FIRE_EXPLOSIVE_PRATICLE_H
