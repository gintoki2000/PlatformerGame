#include "Cell.h"
#include "Constances.h"
#include "Adventurer.h"
#include "SDL_assert.h"
#include "Utils.h"
#include "Vec.h"
#include "WorldManager.h"
#include "box2d/b2_fixture.h"

Cell::Cell() : m_identifier(TAG_BLOCK, this), m_body(nullptr) {}

Cell* Cell::Create(Tile *tile, const Vec2 &center)
{
	Cell* cell = new Cell;
	if (cell->Init(tile, center))
	{
		return cell;
	}
	delete cell;
	return nullptr;
}

bool Cell::Init(Tile* tile, const Vec2& center)
{
    if (tile == nullptr)
    {
        return false;
    }
    SetTile(tile);
    m_identifier.tag    = TAG_BLOCK;
    m_identifier.object = this;
    if (tile->GetNumShapes() != 0)
    {
        b2BodyDef bdef;
        bdef.position.x = center.x / Constances::PPM;
        bdef.position.y = center.y / Constances::PPM;
        bdef.userData   = &m_identifier;

        m_body = WorldManager::GetWorld()->CreateBody(&bdef);

        b2FixtureDef fdef;
        fdef.filter.categoryBits = CATEGORY_BIT_BLOCK;
        fdef.friction            = 0.1f;
        fdef.restitution         = 0.f;
        for (int i = 0; i < tile->GetNumShapes(); ++i)
        {
            fdef.shape = tile->GetShapes()[i];
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

void Cell::OnBeginContact(const ContactInfo&) {}
void Cell::OnEndContact(const ContactInfo&) {}
void Cell::OnPreSolve(const ContactInfo&, const b2Manifold&) {}
void Cell::OnPostSolve(const ContactInfo&, const b2ContactImpulse&) {}
