#include "NTTileLayerAnimatedTile.h"
#include "SDL_assert.h"

Uint32 NTTileLayerAnimatedTile::initialTicks = 0;
Uint32 NTTileLayerAnimatedTile::baseTicks = 0;

NTTileLayerAnimatedTile::NTTileLayerAnimatedTile(const std::vector<NTTextureRegion> &frames, const std::vector<unsigned int> &intervals):
    m_frames(frames), m_intervals(intervals), m_animationDuration(0)
{
	for(const auto& interval : intervals)
			m_animationDuration += interval;
}

const NTTextureRegion& NTTileLayerAnimatedTile::getTextureRegion()
{
		 Uint32 currentTicks = (baseTicks - initialTicks) % m_animationDuration;
		 for(std::size_t i = 0; i < m_frames.size(); ++i)
		 {
				 Uint32 interval = m_intervals[i];
                 if(currentTicks <= interval)
						 return m_frames[i];
				 currentTicks -= interval;
		 }	
}

void NTTileLayerAnimatedTile::initBaseTime()
{
		initialTicks = SDL_GetTicks();
		baseTicks = initialTicks;
}

void NTTileLayerAnimatedTile::updateBaseTime()
{
		baseTicks = SDL_GetTicks();
}
