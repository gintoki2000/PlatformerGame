#include "ParticleSystem.h"
#include "FireBustParticle.h"
#include "Level.h"

ParticleSystem::ParticleSystem(Level* level) : m_level(level) {}

ParticleSystem::~ParticleSystem()
{
    for (auto p : m_pools)
    {
        delete p.second;
    }
    m_pools.clear();
}
