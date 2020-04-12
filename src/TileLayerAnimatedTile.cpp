#include "TileLayerAnimatedTile.h"
#include "SDL_assert.h"

Uint32 TileLayerAnimatedTile::initialTicks = 0;
Uint32 TileLayerAnimatedTile::baseTicks = 0;

TileLayerAnimatedTile::TileLayerAnimatedTile(const std::vector<TextureRegion> &frames, const std::vector<unsigned int> &intervals):
    m_frames(frames), m_intervals(intervals), m_animationDuration(0)
{
	for(const auto& interval : intervals)
			m_animationDuration += interval;
}

const TextureRegion& TileLayerAnimatedTile::getTextureRegion()
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

void TileLayerAnimatedTile::initBaseTime()
{
		initialTicks = SDL_GetTicks();
		baseTicks = initialTicks;
}

void TileLayerAnimatedTile::updateBaseTime()
{
		baseTicks = SDL_GetTicks();
}
