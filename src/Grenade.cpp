#include "Grenade.h"
#include "Animation.h"

#include "Audio.h"
#include "Constances.h"
#include "FireBustParticle.h"
#include "Game.h"
#include "Level.h"
#include "Monster.h"
#include "ObjectLayer.h"
#include "ParticleSystem.h"
#include "SDL_mixer.h"
#include "SDL_render.h"
#include "Scene.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "Utils.h"
#include "WorldManager.h"
#include "box2d/b2_circle_shape.h"

Grenade::Grenade() {}

Grenade::~Grenade()
{
    if (m_body != nullptr)
    {
        m_body->SetUserData(nullptr);
        m_body->GetWorld()->DestroyBody(m_body);
        m_body = nullptr;
    }
}

Grenade* Grenade::create(const Vec2& pos, Direction dir)
{
    Grenade* ret = new Grenade;
    if (ret->init(pos, dir))
    {
        return ret;
    }
    DELETE_NULL(ret);
    return nullptr;
}

bool Grenade::init(const Vec2& pos, Direction dir)
{
    m_sprite = makeSprite(TEX_GRENADE);

    b2BodyDef bdef;
    bdef.position.x    = pos.x / Constances::PPM;
    bdef.position.y    = pos.y / Constances::PPM;
    bdef.type          = b2_dynamicBody;
    bdef.fixedRotation = false;

    float sign            = directionToSign(dir);
    bdef.linearVelocity.x = sign * 5.f;
    bdef.linearVelocity.y = -5.f;

    bdef.angularVelocity = sign * 3.14 * 2.f;

    m_body = WorldManager::getWorld()->CreateBody(&bdef);

    b2CircleShape circle;
    circle.m_radius = 6.f / Constances::PPM;

    b2FixtureDef fdef;
    fdef.shape               = &circle;
    fdef.filter.categoryBits = CATEGORY_BIT_SPELL;
    fdef.filter.maskBits     = CATEGORY_BIT_BLOCK;
    fdef.restitution         = 0.5f;
    fdef.friction            = 0.1f;

    m_body->CreateFixture(&fdef);

    m_timer = 0.f;
    m_flip  = dir == DIRECTION_LEFT ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    return true;
}

void Grenade::tick(float deltaTime)
{
    m_positionX = m_body->GetPosition().x * Constances::PPM;
    m_positionY = m_body->GetPosition().y * Constances::PPM;

    const float RAD_TO_DEG = 57.2957795f;
    m_rotation             = m_body->GetAngle() * RAD_TO_DEG;

    m_timer += deltaTime;

    if (m_timer >= 1.f)
    {
        remove();
        Level* level = static_cast<Level*>(getScene());

        Vec2 position = getPosition() + Vec2(0.f, -20.f);
        level->getParticleSystem()->create<FireBustParticle>(position);
        level->getCameraShaker()->shake(0.1f, 5, 20);

        const int  MAX = 10;
        b2Fixture* fixtures[MAX];
        int        n = 0;
        FloatRect  rect(getPosition() - Vec2(25.f, 25.f), Vec2(50.f, 50.f));
        boxQuery(rect, CATEGORY_BIT_MONSTER, fixtures, n, MAX);

        for (int i = 0; i < n; ++i)
        {
            const Identifier* idr = static_cast<const Identifier*>(
                fixtures[i]->GetBody()->GetUserData());
            Monster* monster = static_cast<Monster*>(idr->object);
            monster->takeDamge(5, DIRECTION_NONE);
        }

        Audio::play(SOUND_EXPLOSION);
    }
}

void Grenade::paint()
{
    SDL_Renderer*   renderer = Game::getInstance()->renderer();
    const SDL_Rect& viewport = getScene()->getCamera().getViewport();

    SDL_Rect dstrect;
    dstrect.x = m_positionX - 6 - viewport.x;
    dstrect.y = m_positionY - 6 - viewport.y;
    dstrect.w = m_sprite.getWidth();
    dstrect.h = m_sprite.getHeight();

    m_sprite.draw(renderer, &dstrect, m_rotation, nullptr, m_flip);
}

void Grenade::cleanup() { delete this; }

void Grenade::onBeginContact(const ContactInfo&) {}
void Grenade::onEndContact(const ContactInfo&) {}
void Grenade::onPreSolve(const ContactInfo&, const b2Manifold&) {}
void Grenade::onPostSolve(const ContactInfo&, const b2ContactImpulse&) {}
