#ifndef FIRE_BUST_PARTICLE_H
#define FIRE_BUST_PARTICLE_H
#include "AnimatedParticle.h"
#include "Animation.h"
#include "Animator.h"
#include "GameObject.h"
#include "Pool.h"
#include "SpriteSheet.h"
class FireBustParticle : public AnimatedParticle,
                         public PoolableObject<FireBustParticle>
{
    /// GameObject interface
  public:
    void cleanup() override;

  private:
    bool preinit();

    static constexpr int SPRITE_WIDTH  = 64;
    static constexpr int SPRITE_HEIGHT = 64;

    friend class AnimatedParticlePool<FireBustParticle>;
};

#endif // FIRE_BUST_PARTICLE_H
