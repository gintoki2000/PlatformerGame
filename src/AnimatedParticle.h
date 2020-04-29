#ifndef ANIMATED_PARTICLE_H
#define ANIMATED_PARTICLE_H
#include "Animation.h"
#include "AssertManager.h"
#include "Particle.h"
#include "Pool.h"
#include "SpriteSheet.h"
class AnimatedParticle : public Particle
{
  public:
    AnimatedParticle();
    ~AnimatedParticle();

    void         init(const Vec2& position);
    virtual bool preinit(TextureManager::TextureID textureID,
                         float frameDuration, int spriteWidth,
                         int spriteHeight);

    // GameObject interface
  public:
    void tick(float deltaTime) override;
    void paint() override;

  private:
    SpriteSheet m_spriteSheet;
    Animation*  m_animation;
    float       m_timer;
};

template<typename TObject>
class AnimatedParticlePool : public BasePool<TObject>
{
	public:
		static AnimatedParticlePool* create(int numObjects)
		{
			AnimatedParticlePool* ret = new AnimatedParticlePool;
			if (ret->init(numObjects))
			{
				return ret;
			}
			delete ret;
			return nullptr;
		}	

	protected:
		virtual bool init(int numObjects) override
		{
			if (!BasePool<TObject>::init(numObjects))
			{
				return false;
			}
			this->m_numObjects = numObjects;
			for (int i = 0; i < this->m_numObjects; ++i)
			{
				this->m_objects[i].preinit();
			}
		}
};
#endif // ANIMATED_PARTICLE_H
