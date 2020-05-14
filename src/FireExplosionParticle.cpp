#include "FireExplosionParticle.h"

void FireExplosionParticle::cleanup() { getPool()->free(this); }

