#ifndef PARTICLE_SYSTEm_H
#define PARTICLE_SYSTEm_H
#include "Level.h"
#include "Pool.h"
#include "ObjectLayer.h"
#include "SDL_assert.h"
#include <typeindex>
#include <unordered_map>

class ParticleSystem
{
  public:
    ParticleSystem(Level* level);
    ~ParticleSystem();

    template <typename TParticle, typename TPool, typename... TArgs>
    void Resgiter(TArgs... args)
    {
        std::type_index tidx(typeid(TParticle));
        auto            findResult = m_pools.find(tidx);
        if (findResult == std::end(m_pools))
        {
            IPool* pool = new TPool(args...);
            m_pools.insert(std::make_pair(tidx, pool));
        }
    }

    template <typename TParticle, typename... TArgs> void Create(const TArgs&... args)
    {
        IPool* pool = GetPool<TParticle>();
        SDL_assert(pool != nullptr);
        TParticle* particle = static_cast<TParticle*>(pool->Alloc());
        particle->Init(args...);
		m_level->GetParticleLayer()->AddObject(particle);
    }

    template <typename TParticle> IPool* GetPool()
    {
        auto findResult = m_pools.find(std::type_index(typeid(TParticle)));
        return findResult != std::end(m_pools) ? findResult->second : nullptr;
    }

  private:
    std::unordered_map<std::type_index, IPool*> m_pools;
    Level*                                      m_level;
};
#endif // PARTICLE_SYSTEm_H
