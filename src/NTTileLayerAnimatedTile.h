#ifndef NTTILEDMAPANIMATEDTILE_H
#define NTTILEDMAPANIMATEDTILE_H
#include "NTTileLayerTile.h"
#include <vector>
class NTTileLayerAnimatedTile : public NTTileLayerTile
{
public:
    NTTileLayerAnimatedTile(const std::vector<NTTextureRegion>& frames, const std::vector<Uint32>& intervals);

    virtual const NTTextureRegion& getTextureRegion() override;

	static void initBaseTime();

	static void updateBaseTime();

private:
    std::vector<NTTextureRegion> m_frames;
    std::vector<Uint32> m_intervals;
    Uint32 m_animationDuration;
    static Uint32 initialTicks;
    static Uint32 baseTicks;
};

#endif // NTTILEDMAPANIMATEDTILE_H
