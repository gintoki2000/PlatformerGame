#include "Axe.h"

#include "Audio.h"
#include "Constances.h"
#include "Game.h"
#include "Level.h"
#include "Monster.h"
#include "SDL_mixer.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include "Scene.h"
#include "SpriteSheet.h"
#include "TextureManager.h"
#include "Utils.h"
#include "WorldManager.h"
#include "box2d/b2_circle_shape.h"

Axe::Axe() : m_body(nullptr), m_identifier(TAG_SPELL, this) {}

Axe::~Axe()
{
    if (m_body != nullptr)
    {
        m_body->SetUserData(nullptr);
        m_body->GetWorld()->DestroyBody(m_body);
        m_body = nullptr;
    }
}

Axe* Axe::create(const Vec2& pos, Direction adventurerDir)
{
    Axe* ret = new Axe;
    if (ret->init(pos, adventurerDir))
    {
        return ret;
    }
    DELETE_NULL(ret);
    return nullptr;
}

void Axe::tick(float)
{
    ++m_frameCounter;
    m_positionX = m_body->GetPosition().x * Constances::PPM;
    m_positionY = m_body->GetPosition().y * Constances::PPM;
    m_rotation  = m_body->GetAngle() * Constances::RAD_TO_DEG;

    if (m_frameCounter >= 3)
    {
        m_frameCounter -= 3;
        for (int i = NUM_SHADOWS - 1; i > 0; --i)
        {
            m_shadows[i] = m_shadows[i - 1];
        }
        m_shadows[0] = {m_positionX, m_positionY, (float)m_rotation};
    }

    SDL_Rect boundingBox;
    boundingBox.x            = m_positionX - 16;
    boundingBox.y            = m_positionY - 16;
    boundingBox.w            = 32;
    boundingBox.h            = 32;
    const SDL_Rect& viewport = getScene()->getCamera().getViewport();
    if (!SDL_HasIntersection(&boundingBox, &viewport))
    {
        remove();
    }
}

void Axe::paint()
{

    SDL_Renderer*   renderer = GAME->renderer();
    const SDL_Rect& viewport = getScene()->getCamera().getViewport();

    SDL_Rect dstrect;
    dstrect.w = m_sprite.getWidth();
    dstrect.h = m_sprite.getHeight();

    Uint8 d = 255 / (NUM_SHADOWS + 1);
    Uint8 a = d;
    for (int i = NUM_SHADOWS - 1; i >= 0; --i)
    {
        SDL_SetTextureAlphaMod(m_sprite.texture, a);
        dstrect.x = m_shadows[i].x - 16 - viewport.x;
        dstrect.y = m_shadows[i].y - 16 - viewport.y;
        m_sprite.draw(renderer, &dstrect, m_shadows[i].r, nullptr,
                      SDL_FLIP_NONE);
        a += d;
    }
    dstrect.x = m_positionX - 16 - viewport.x;
    dstrect.y = m_positionY - 16 - viewport.y;
    SDL_SetTextureAlphaMod(m_sprite.texture, 255);
    m_sprite.draw(renderer, &dstrect, m_rotation, nullptr, SDL_FLIP_NONE);
}

void Axe::cleanup() { delete this; }

bool Axe::init(const Vec2& pos, Direction dir)
{
    SDL_Texture* texture = TextureManager::get(TEX_THROWING_AXE);
    if (texture == nullptr)
    {
        return false;
    }
    m_sprite = Sprite(texture);
    b2BodyDef bdef;
    bdef.type       = b2_dynamicBody;
    bdef.bullet     = true;
    bdef.angle      = 100.f;
    bdef.userData   = &m_identifier;
    bdef.position.x = pos.x / Constances::PPM;
    bdef.position.y = pos.y / Constances::PPM;

    m_body = WorldManager::getWorld()->CreateBody(&bdef);

    b2CircleShape circle;
    circle.m_radius = 16.f / Constances::PPM;

    b2FixtureDef fdef;
    fdef.shape               = &circle;
    fdef.filter.categoryBits = CATEGORY_BIT_SPELL;
    fdef.filter.maskBits     = CATEGORY_BIT_MONSTER;
    fdef.isSensor            = false;

    m_body->CreateFixture(&fdef);

    int sign = directionToSign(dir);
    m_body->SetAngularVelocity(sign * 2.f * 3.141592f);
    m_body->SetAngularDamping(0.f);

    b2Vec2 vel;
    vel.x = sign * 4.f;
    vel.y = -20.f;
    m_body->SetLinearVelocity(vel);

    for (int i = 0; i < NUM_SHADOWS; ++i)
    {
        m_shadows[i].x = pos.x;
        m_shadows[i].y = pos.y;
        m_shadows[i].r = 0.f;
    }

    m_frameCounter = 0;
    return true;
}

void Axe::onBeginContact(const ContactInfo&) {}

void Axe::onEndContact(const ContactInfo&) {}
void Axe::onPreSolve(const ContactInfo& info, const b2Manifold&)
{
    const Identifier* idr = info.getOtherIdentifier();
    if (idr != nullptr && idr->tag == TAG_MONSTER)
    {
        Monster* monster = static_cast<Monster*>(idr->object);
        if (monster->takeDamge(5, DIRECTION_NONE))
        {
			Audio::play(SOUND_STAB);
        }
    }
    info.setIsEnabled(false);
}
void Axe::onPostSolve(const ContactInfo&, const b2ContactImpulse&) {}
