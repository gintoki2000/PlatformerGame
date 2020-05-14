#ifndef BLOODSTAIN_PARTICLE_H
#define BLOODSTAIN_PARTICLE_H
#include "AnimatedParticle.h"
#include "Animation.h"
#include "GameObject.h"
#include "Pool.h"
#include "SpriteSheet.h"
#include "TextureManager.h"
#include "Vec.h"
class BloodStainParticle : public AnimatedParticle,
                           public PoolableObject<BloodStainParticle>
{
  public:
    typedef AnimatedParticlePool<BloodStainParticle> Pool;
    // GameObject interface
  public:
    void cleanup() override { getPool()->free(this); }

    static constexpr int   SPRITE_WIDTH   = 100;
    static constexpr int   SPRITE_HEIGHT  = 100;
    static constexpr float FRAME_DURATION = 1.f / 30.f;
    static constexpr int   TEXTURE_ID     = TEX_BLOOD_STAIN;
};
#endif // BLOODSTAIN_PARTICLE_H
