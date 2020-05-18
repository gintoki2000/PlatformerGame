#ifndef FIRE_EXPLOSION_PRATICLE_H
#define FIRE_EXPLOSION_PRATICLE_H
#include "AnimatedParticle.h"
#include "Pool.h"
class FireExplosionParticle : public AnimatedParticle,
                              public PoolableObject<FireExplosionParticle>
{
    // GameObject interface
  public:
    typedef AnimatedParticlePool<FireExplosionParticle> Pool;
    void Cleanup() override;

  private:
    friend Pool;

    static constexpr int   SPRITE_WIDTH   = 96;
    static constexpr int   SPRITE_HEIGHT  = 96;
    static constexpr float FRAME_DURATION = 0.02f;
};
#endif // FIRE_EXPLOSIVE_PRATICLE_H
