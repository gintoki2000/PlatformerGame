#include "FireExplosionParticle.h"

void FireExplosionParticle::Cleanup() { GetPool()->Free(this); }

