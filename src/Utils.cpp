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

bool BoxCast(const FloatRect& rect, uint16 maskBits)
{
    b2World&            world = *WorldManager::GetWorld();
    OverlapTestCallback callback;
    callback.maskBits = maskBits;
    b2AABB box;
    box.lowerBound.x = rect.x / Constances::PPM;
    box.lowerBound.y = rect.y / Constances::PPM;
    box.upperBound.x = rect.Right() / Constances::PPM;
    box.upperBound.y = rect.Bottom() / Constances::PPM;
    world.QueryAABB(&callback, box);

    return callback.fixture != nullptr;
}

struct QueryCallback : public b2QueryCallback
{
    b2Fixture** fixtures;
    int         max;
    int         n;
    uint16      maskBits;

  public:
    QueryCallback(b2Fixture** _fixtures, int max, uint16 _maskBits = 0xffff);

    // b2QueryCallback interface
  public:
    bool ReportFixture(b2Fixture* fixture) override;
};

QueryCallback::QueryCallback(b2Fixture** _fixtures, int _max,
                             uint16 _maskBits) :
    fixtures(_fixtures),
    max(_max), n(0), maskBits(_maskBits)
{
}

bool QueryCallback::ReportFixture(b2Fixture* fixture)
{
    if (fixture->GetFilterData().categoryBits & maskBits)
    {
        fixtures[n++] = fixture;
    }
    return n < max;
}

void BoxQuery(const FloatRect& rect, uint16 maskBits, b2Fixture* fixtures[], int& n,
              const int max)
{

    b2World&      world = *WorldManager::GetWorld();
    QueryCallback queryCallback(fixtures, max, maskBits);
    b2AABB        box;
    box.lowerBound.x = rect.x / Constances::PPM;
    box.lowerBound.y = rect.y / Constances::PPM;
    box.upperBound.x = rect.Right() / Constances::PPM;
    box.upperBound.y = rect.Bottom() / Constances::PPM;
    world.QueryAABB(&queryCallback, box);

    n = queryCallback.n;
}

int DirectionToSign(Direction direction)
{
	return direction == DIRECTION_LEFT ? -1 : 1;
}


Direction RelativeDirection(float x1, float x2)
{
	if (x1 > x2)
		return DIRECTION_LEFT;
	if (x1 < x2)
		return DIRECTION_RIGHT;
	return DIRECTION_NONE;
}

int RandomRange(int a, int b)
{
	int d = b - a + 1;
	return rand() % d + a + 1; 
}
