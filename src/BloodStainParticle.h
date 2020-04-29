#ifndef BLOODSTAIN_PARTICLE_H
#define BLOODSTAIN_PARTICLE_H
#include "Animation.h"
#include "GameObject.h"
#include "Pool.h"
#include "SpriteSheet.h"
#include "Vec.h"
class BloodStainParticle : public GameObject,
                           public PoolableObject<BloodStainParticle>
{
  public:
    void init(const Vec2& position);

    // GameObject interface
  public:
    void tick(float deltaTime) override;
    void paint() override;
    void cleanup() override;

  private:
    BloodStainParticle();
    ~BloodStainParticle();

    static constexpr int SPRITE_WIDTH  = 100;
    static constexpr int SPRITE_HEIGHT = 100;

    bool        preinit();
    SpriteSheet m_spritesheet;
    Animation*  m_animation;
    float       m_timer;

    friend class BasePool<BloodStainParticle>;
};
#endif // BLOODSTAIN_PARTICLE_H
