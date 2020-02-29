#include "NTTiledMapAnimatedTile.h"
#include "SDL_assert.h"

unsigned int NTTiledMapAnimatedTile::initialTicks = 0;
unsigned int NTTiledMapAnimatedTile::baseTicks = 0;

NTTiledMapAnimatedTile::NTTiledMapAnimatedTile(const std::vector<NTTextureRegion> &frames, const std::vector<unsigned int> &intervals):
    m_frames(frames), m_intervals(intervals)
{

}

const NTTextureRegion& NTTiledMapAnimatedTile::getTextureRegion()
{
		 Uint32 currentTicks = (baseTicks - initialTicks) % m_animationDuration;
		 for(std::size_t i = 0; i < m_frames.size(); ++i)
		 {
				 Uint32 interval = m_intervals[i];
				 if(interval <= currentTicks)
						 return m_frames[i];
				 currentTicks -= interval;
		 }	
}

void NTTiledMapAnimatedTile::initBaseTime()
{
		initialTicks = SDL_GetTicks();
		baseTicks = initialTicks;
}

void NTTiledMapAnimatedTile::updateBaseTime()
{
		baseTicks = SDL_GetTicks();
}
