#ifndef ANIMATED_PARTICLE_H
#define ANIMATED_PARTICLE_H
#include "Animation.h"
#include "Particle.h"
#include "Pool.h"
#include "SDL_render.h"
#include "SpriteSheet.h"
#include "TextureManager.h"

struct AnimatedParticleModel
{
    SpriteSheet spriteSheet;
    Animation*  animation;
};

template <typename TObject> class AnimatedParticlePool;

class AnimatedParticle : public Particle
{
  public:
    AnimatedParticle();
    ~AnimatedParticle();

    void init(const Vec2& position);

    // GameObject interface
  public:
    void tick(float deltaTime) override;
    void paint() override;

  public:
    AnimatedParticleModel* m_model;

  private:
    float m_timer;
};

template <typename TObject>
class AnimatedParticlePool : public BasePool<TObject>
{
  public:
    AnimatedParticlePool(int numObjects) : BasePool<TObject>(numObjects)
    {
        SDL_Texture* texture = TextureManager::get(TObject::TEXTURE_ID);
        m_model.spriteSheet.init(texture, TObject::SPRITE_WIDTH,
                                 TObject::SPRITE_HEIGHT);
        m_model.animation =
            new Animation(&m_model.spriteSheet, TObject::FRAME_DURATION);
        for (int i = 0; i < this->m_numObjects; ++i)
        {
            this->m_objects[i].m_model = &m_model;
        }
    }
    ~AnimatedParticlePool() { delete m_model.animation; }

  private:
    AnimatedParticleModel m_model;
};

#endif // ANIMATED_PARTICLE_H
