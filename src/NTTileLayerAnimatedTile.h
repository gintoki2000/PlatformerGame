#ifndef NTTILEDMAPANIMATEDTILE_H
#define NTTILEDMAPANIMATEDTILE_H
#include "NTTileLayerTile.h"
#include <vector>
class TileLayerAnimatedTile : public NTTileLayerTile
{
public:
    TileLayerAnimatedTile(const std::vector<TextureRegion>& frames, const std::vector<Uint32>& intervals);

    virtual const TextureRegion& getTextureRegion() override;

	static void initBaseTime();

	static void updateBaseTime();

private:
    std::vector<TextureRegion> m_frames;
    std::vector<Uint32> m_intervals;
    Uint32 m_animationDuration;
    static Uint32 initialTicks;
    static Uint32 baseTicks;
};

#endif // NTTILEDMAPANIMATEDTILE_H
