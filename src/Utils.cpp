#include "Utils.h"
#include "Constances.h"
#include "WorldManager.h"

struct OverlapTestCallback : public b2QueryCallback
{
    uint16     maskBits;
    b2Fixture* fixture;
    OverlapTestCallback()
    {
        maskBits = 0;
        fixture  = nullptr;
    }

    bool ReportFixture(b2Fixture* f) override
    {
        if (f->GetFilterData().categoryBits & maskBits)
        {
            this->fixture = f;
            return false;
        }
        return true;
    }
};

bool boxCast(const Rect& rect, uint16 maskBits)
{
    b2World&            world = *WorldManager::getWorld();
    OverlapTestCallback callback;
    callback.maskBits = maskBits;
    b2AABB box;
    box.lowerBound.x = rect.left() / Constances::PPM;
    box.lowerBound.y = rect.top() / Constances::PPM;
    box.upperBound.x = rect.right() / Constances::PPM;
    box.upperBound.y = rect.bottom() / Constances::PPM;
    world.QueryAABB(&callback, box);

    return callback.fixture != nullptr;
}
