#include "Fireball.h"
#include "Audio.h"
#include "FireExplosionParticle.h"
#include "ParticleSystem.h"
#include "Animation.h"
#include "Camera.h"
#include "Constances.h"
#include "Game.h"
#include "Scene.h"
#include "Level.h"
#include "Monster.h"
#include "SDL_mixer.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "TextureManager.h"
#include "Utils.h"
#include "Vec.h"
#include "WorldManager.h"
#include "box2d/b2_body.h"
#include "box2d/b2_circle_shape.h"
Fireball::Fireball()
{
    m_body              = nullptr;
    m_animation         = nullptr;
    m_identifier.object = this;
}

Fireball::~Fireball()
{
    if (m_body != nullptr)
    {
        m_body->SetUserData(nullptr);
        m_body->GetWorld()->DestroyBody(m_body);
        m_body = nullptr;
    }
    DELETE_NULL(m_animation);
}

Fireball* Fireball::Create(const Vec2& position, Direction direction,
                           float speed)
{
    Fireball* ret = new Fireball;
    if (ret->Init(position, direction, speed))
    {
        return ret;
    }
    DELETE_NULL(ret);
    return nullptr;
}

Fireball* Fireball::Create(const Vec2& position, Direction direction)
{
    return Create(position, direction, 10.f);
}

void Fireball::Start()
{
	Audio::Play(SOUND_FIREBALL);
}

void Fireball::Cleanup() { delete this; }

bool Fireball::Init(const Vec2& position, Direction direction, float speed)
{
    SDL_Texture* texture = TextureManager::Get(TEX_FIREBALL);
    m_spriteSheet.Init(texture, 64, 64);
    m_animation =
        new Animation(&m_spriteSheet, 0.06f, Animation::PLAY_MODE_LOOP);
    m_flip = direction == DIRECTION_LEFT ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    b2BodyDef bdef;
    bdef.bullet           = true;
    bdef.position.x       = position.x / Constances::PPM;
    bdef.position.y       = position.y / Constances::PPM;
    bdef.fixedRotation    = true;
    bdef.gravityScale     = 0.f;
    bdef.type             = b2_dynamicBody;
    bdef.linearVelocity.y = 0;
    bdef.userData         = &m_identifier;

    float sign            = direction == DIRECTION_LEFT ? -1.f : 1.f;
    bdef.linearVelocity.x = speed * sign;

    m_body = WorldManager::GetWorld()->CreateBody(&bdef);

    b2CircleShape circle;
    circle.m_radius = 8.f / Constances::PPM;

    b2FixtureDef fdef;
    fdef.filter.categoryBits = CATEGORY_BIT_PROJECTILE;
    fdef.filter.maskBits     = CATEGORY_BIT_MONSTER | CATEGORY_BIT_ADVENTURER;
    fdef.isSensor            = true;
    fdef.shape               = &circle;
    m_body->CreateFixture(&fdef);

    return true;
}

void Fireball::Tick(float deltaTime)
{
    static SDL_Rect boundingBox = {0, 0, 16, 16};

    m_positionX = m_body->GetPosition().x * Constances::PPM;
    m_positionY = m_body->GetPosition().y * Constances::PPM;

    m_timer += deltaTime;
    boundingBox.x            = m_positionX - 8;
    boundingBox.y            = m_positionY - 8;
    const Camera&   camera   = GetScene()->GetCamera();
    const SDL_Rect& viewport = camera.GetViewport();

    if (!SDL_HasIntersection(&boundingBox, &viewport))
    {
        Remove();
    }
}

void Fireball::Paint()
{

    const Sprite& sprite = *(m_animation->GetCurrentSprite(m_timer));

    const Camera& camera = GetScene()->GetCamera();

    const SDL_Rect& viewport = camera.GetViewport();

    SDL_Renderer* renderer = GAME->GetRenderer();

    SDL_Rect dstrect;
    dstrect.x = m_positionX - sprite.GetWidth() / 2 - viewport.x;
    dstrect.y = m_positionY - sprite.GetHeight() / 2 - viewport.y;
    dstrect.w = sprite.GetWidth();
    dstrect.h = sprite.GetHeight();
    sprite.Draw(renderer, &dstrect, 0.0, nullptr, m_flip);
}

void Fireball::OnBeginContact(const ContactInfo& info)
{
    const Identifier* idr = info.GetOtherIdentifier();
    if (idr != nullptr && idr->tag == TAG_MONSTER)
    {
        Monster* monster = static_cast<Monster*>(idr->object);
        if (monster->TakeDamge(1, DIRECTION_NONE))
        {
            Remove();
        }
    }
}

void Fireball::OnEndContact(const ContactInfo&) {}
void Fireball::OnPreSolve(const ContactInfo&, const b2Manifold&) {}
void Fireball::OnPostSolve(const ContactInfo&, const b2ContactImpulse&) {}
