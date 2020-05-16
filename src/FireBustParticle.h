#ifndef FIRE_BUST_PARTICLE_H
#define FIRE_BUST_PARTICLE_H
#include "AnimatedParticle.h"
#include "Animation.h"
#include "Animator.h"
#include "GameObject.h"
#include "Pool.h"
#include "SpriteSheet.h"
#include "TextureManager.h"

class FireBustParticle : public AnimatedParticle,
                         public PoolableObject<FireBustParticle>
{
    // GameObject interface
  public:
    typedef AnimatedParticlePool<FireBustParticle> Pool;
    void Cleanup() override { GetPool()->Free(this); }

  public:
    static constexpr int   SPRITE_WIDTH   = 64;
    static constexpr int   SPRITE_HEIGHT  = 64;
    static constexpr float FRAME_DURATION = 1.f / 20.f;
    static constexpr int   TEXTURE_ID     = TEX_FIRE_BUST;
};

#endif // FIRE_BUST_PARTICLE_H
