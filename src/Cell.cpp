#include "Cell.h"
#include "Constances.h"
#include "Player.h"
#include "SDL_assert.h"
#include "Utils.h"
#include "Vec.h"
#include "WorldManager.h"
#include "box2d/b2_fixture.h"

Cell::Cell() : m_identifier(TAG_BLOCK, this), m_body(nullptr) {}

Cell* Cell::create(Tile *tile, const Vec2 &center)
{
	Cell* cell = new Cell;
	if (cell->init(tile, center))
	{
		return cell;
	}
	delete cell;
	return nullptr;
}

bool Cell::init(Tile* tile, const Vec2& center)
{
    if (tile == nullptr)
    {
        return false;
    }
    setTile(tile);
    m_identifier.tag    = TAG_BLOCK;
    m_identifier.object = this;
    if (tile->getNumShapes() != 0)
    {
        b2BodyDef bdef;
        bdef.position.x = center.x / Constances::PPM;
        bdef.position.y = center.y / Constances::PPM;
        bdef.userData   = &m_identifier;

        m_body = WorldManager::getWorld()->CreateBody(&bdef);

        b2FixtureDef fdef;
        fdef.filter.categoryBits = CATEGORY_BIT_BLOCK;
        fdef.friction            = 0.1f;
        fdef.restitution         = 0.f;
        for (int i = 0; i < tile->getNumShapes(); ++i)
        {
            fdef.shape = tile->getShapes()[i];
            m_body->CreateFixture(&fdef);
        }
    }
    return true;
}

Cell::~Cell()
{
    if (m_body != nullptr)
    {
        m_body->SetUserData(nullptr);
        m_body->GetWorld()->DestroyBody(m_body);
        m_body = nullptr;
    }
}

void Cell::onBeginContact(const ContactInfo&) {}
void Cell::onEndContact(const ContactInfo&) {}
void Cell::onPreSolve(const ContactInfo&, const b2Manifold&) {}
void Cell::onPostSolve(const ContactInfo&, const b2ContactImpulse&) {}
