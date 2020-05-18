#include "PickupObject.h"
#include "Camera.h"
#include "Constances.h"
#include "Game.h"
#include "SDL_rect.h"
#include "Scene.h"
#include "Utils.h"
#include "WorldManager.h"
#include "box2d/b2_polygon_shape.h"
static void RenderSprite(const Sprite& sprite, GameObject* gameObject)
{
    const SDL_Rect& view = gameObject->GetScene()->GetCamera().GetViewport();
    SDL_Rect        dstrect;
    dstrect.x = gameObject->GetPositionX() - sprite.GetWidth() / 2;
    dstrect.y = gameObject->GetPositionY() - sprite.GetHeight() / 2;
    dstrect.w = sprite.GetWidth();
    dstrect.h = sprite.GetHeight();
    if (SDL_HasIntersection(&view, &dstrect))
    {
        dstrect.x -= view.x;
        dstrect.y -= view.y;
        sprite.Draw(GAME->GetRenderer(), &dstrect);
    }
}

PickupObject::PickupObject(const Sprite& sprite, const Vec2& position) :
    m_sprite(sprite)
{
    b2BodyDef bdef;
    bdef.position.x    = position.x / Constances::PPM;
    bdef.position.y    = position.y / Constances::PPM;
    bdef.userData      = &m_identifier;
    bdef.fixedRotation = true;
    bdef.type          = b2_dynamicBody;

    m_body = WorldManager::GetWorld()->CreateBody(&bdef);

    b2PolygonShape box;
    box.SetAsBox(sprite.GetWidth() / 2.f / Constances::PPM,
                 sprite.GetHeight() / 2.f / Constances::PPM);

    b2FixtureDef fdef;
    fdef.friction            = 1.f;
    fdef.restitution         = 0.f;
    fdef.shape               = &box;
    fdef.filter.categoryBits = CATEGORY_BIT_OTHER_OBJECT;
    fdef.filter.maskBits     = CATEGORY_BIT_ADVENTURER | CATEGORY_BIT_BLOCK;

    m_body->CreateFixture(&fdef);

    SetPosition(position);
    m_identifier.tag    = TAG_OTHER_OBJECT;
    m_identifier.object = this;
}

PickupObject::~PickupObject()
{
    m_body->GetWorld()->DestroyBody(m_body);
    m_body = nullptr;
}

void PickupObject::Start() {}

void PickupObject::Tick(float)
{
    m_positionX = m_body->GetPosition().x * Constances ::PPM;
    m_positionY = m_body->GetPosition().y * Constances ::PPM;
}

void PickupObject::Paint() { RenderSprite(m_sprite, this); }

void PickupObject::Cleanup() { delete this; }

void PickupObject::OnPreSolve(const ContactInfo& info, const b2Manifold&)
{
    const Identifier* idr = info.GetOtherIdentifier();
    if (idr->tag == TAG_ADVENTURER)
    {
        OnCollected();
        Remove();
    	info.SetIsEnabled(false);
    }
}

void PickupObject::OnBeginContact(const ContactInfo&) {}

void PickupObject::OnEndContact(const ContactInfo&) {}

void PickupObject::OnPostSolve(const ContactInfo&, const b2ContactImpulse&) {}

void PickupObject::OnCollected() {}
