#include "Fireball.h"
#include "Game.h"
#include "Animation.h"
#include "AssertManager.h"
#include "Camera.h"
#include "Constances.h"
#include "LayerManager.h"
#include "Monster.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "Utils.h"
#include "WorldManager.h"
#include "box2d/b2_body.h"
#include "box2d/b2_circle_shape.h"
FireBall::FireBall()
{
    m_body              = nullptr;
    m_animation         = nullptr;
    m_identifier.object = this;
}

FireBall::~FireBall()
{
    if (m_body != nullptr)
        m_body->GetWorld()->DestroyBody(m_body);
    m_body = nullptr;
    DELETE_NULL(m_animation);
}

FireBall* FireBall::create(const Vec2& position, Direction direction,
                           float speed)
{
    FireBall* ret = new FireBall;
    if (ret->init(position, direction, speed))
    {
        return ret;
    }
    DELETE_NULL(ret);
    return nullptr;
}

void FireBall::cleanup() { delete this; }

bool FireBall::init(const Vec2& position, Direction direction, float speed)
{
	SDL_Texture* texture = Game::getInstance()->textureMGR().getTexture(TextureManager::TEXTURE_FIREBALL);
    m_spriteSheet.init(texture, 64, 64);
    m_animation = new Animation(&m_spriteSheet, 0.2, Animation::PLAY_MODE_LOOP);
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

    m_body = WorldManager::getWorld()->CreateBody(&bdef);

    b2CircleShape circle;
    circle.m_radius = 8.f / Constances::PPM;

    b2FixtureDef fdef;
    fdef.filter.categoryBits = CATEGORY_BIT_SPELL;
    fdef.filter.maskBits     = CATEGORY_BIT_MONSTER | CATEGORY_BIT_PLAYER;
    fdef.isSensor            = true;
    fdef.shape               = &circle;
    m_body->CreateFixture(&fdef);
    return true;
}

void FireBall::tick(float deltaTime)
{
    static SDL_Rect boundingBox = {0, 0, 16, 16};

    m_positionX = m_body->GetPosition().x * Constances::PPM;
    m_positionY = m_body->GetPosition().y * Constances::PPM;

    m_timer += deltaTime;
    boundingBox.x            = m_positionX - 8;
    boundingBox.y            = m_positionY - 8;
    const Camera&   camera   = getLayerManager()->getCamera();
    const SDL_Rect& viewport = camera.getViewport();

    if (!SDL_HasIntersection(&boundingBox, &viewport))
    {
        setNeedToRemove(true);
    }
}

void FireBall::paint()
{

    const Sprite& sprite = *(m_animation->getCurrentSprite(m_timer));

    const Camera& camera = getLayerManager()->getCamera();

    const SDL_Rect& viewport = camera.getViewport();

    SDL_Rect dstrect;
    dstrect.x = m_positionX - sprite.getWidth() / 2 - viewport.x;
    dstrect.y = m_positionY - sprite.getHeight() / 2 - viewport.y;
    dstrect.w = sprite.getWidth();
    dstrect.h = sprite.getHeight();
    sprite.draw(Game::getInstance()->renderer(), &dstrect, 0.0, nullptr, m_flip);
}

void FireBall::onBeginContact(const ContactInfo& info)
{
    if (info.getOtherIdentifier()->tag == TAG_MONSTER)
    {
        Monster* monster =
            static_cast<Monster*>(info.getOtherIdentifier()->object);
        monster->takeDamge(1);
		setNeedToRemove(true);
    }
}

void FireBall::onEndContact(const ContactInfo&) {}
void FireBall::onPreSolve(const ContactInfo&, const b2Manifold&) {}
void FireBall::onPostSolve(const ContactInfo& info, const b2ContactImpulse&)
{
}
