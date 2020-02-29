#ifndef NTTILEDMAPANIMATEDTILE_H
#define NTTILEDMAPANIMATEDTILE_H
#include "NTTiledMapTile.h"
#include <vector>
class NTTiledMapAnimatedTile : public NTTiledMapTile
{
public:
    NTTiledMapAnimatedTile(const std::vector<NTTextureRegion>& frames, const std::vector<unsigned int>& intervals);

    const NTTextureRegion& getTextureRegion() override;

	static void initBaseTime();

	static void updateBaseTime();

private:
    std::vector<NTTextureRegion> m_frames;
    std::vector<unsigned int> m_intervals;
	unsigned int m_animationDuration;
	static unsigned int initialTicks;
	static unsigned int baseTicks;
};

#endif // NTTILEDMAPANIMATEDTILE_H
