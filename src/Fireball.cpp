#include "Fireball.h"
#include "Audio.h"
#include "Constances.h"
#include "Enums.h"
#include "GameObject.h"
#include "Level.h"
#include "Locator.h"
#include "SDL_rect.h"
#include "SDL_stdinc.h"
#include "SpriteSheet.h"
Fireball::Fireball(Level* level, const b2Vec2& initialPosition,
                   const b2Vec2& speed) :
    GameObject(GAME_OBJECT_TYPE_SPELL, level)
{
    SDL_Texture* texture =
        level->getTextureManager()->get("asserts/spritesheets/fire-ball.png");
    m_spriteSheet = new SpriteSheet(texture, SPRITE_WIDTH, SPRITE_HEIGHT);
    m_origin.x    = SPRITE_WIDTH / 2;
    m_origin.y    = SPRITE_HEIGHT / 2;
    m_animation =
        new Animation(m_spriteSheet, 1.f / 10.f, Animation::PLAY_MODE_LOOP);

    b2BodyDef bdef;
    bdef.position       = initialPosition;
    bdef.linearVelocity = speed;
    bdef.fixedRotation  = true;
    bdef.gravityScale   = 0.f;
    bdef.userData       = this;
    bdef.type           = b2_dynamicBody;
    bdef.angle          = SDL_atan2f(speed.y, speed.x) * 57.29578f;

    m_body = m_level->getWorld()->CreateBody(&bdef);

    // b2PolygonShape box;
    // box.SetAsBox(16.f / 2.f / Constances::PPM, 16.f / 2.f / Constances::PPM);

    b2CircleShape cricle;
    cricle.m_radius = 16.f / 2.f / Constances::PPM;

    b2FixtureDef fdef;
    fdef.filter.categoryBits = CATEGORY_BIT_SPELL;
    fdef.filter.maskBits     = CATEGORY_BIT_MONSTER;
    fdef.shape               = &cricle;
    fdef.isSensor            = true;
    m_body->CreateFixture(&fdef);
    m_animElapsedTime = 0.f;

	Locator::getAudio().play(Audio::FIREBALL);
}

Fireball::~Fireball()
{
    delete m_animation;
    delete m_spriteSheet;
    m_body->GetWorld()->DestroyBody(m_body);
}

void Fireball::tick(float deltaTime)
{
    // update physics
    m_positionX = m_body->GetPosition().x * Constances::PPM;
    m_positionY = m_body->GetPosition().y * Constances::PPM;
    m_rotation  = m_body->GetAngle();

    SDL_Rect boundingBox;
    boundingBox.x = m_positionX - 16.f / 2.f;
    boundingBox.y = m_positionY - 16.f / 2.f;
    boundingBox.w = 16.f;
    boundingBox.h = 16.f;

    const SDL_Rect& viewport = m_level->getViewport();
    if (!SDL_HasIntersection(&boundingBox, &viewport))
    {
        m_level->removeFireball(this);
        return;
    }
    // update graphics
    m_animElapsedTime += deltaTime;
}

void Fireball::paint()
{

    const SDL_Rect& viewport = m_level->getViewport();
    SDL_Rect        dstrect;
    dstrect.x = m_positionX - viewport.x - m_origin.x;
    dstrect.y = m_positionY - viewport.y - m_origin.y;
    dstrect.w = SPRITE_WIDTH;
    dstrect.h = SPRITE_HEIGHT;

    const Sprite* sprite = m_animation->getCurrentSprite(m_animElapsedTime);
    sprite->draw(Locator::getRenderer(), &dstrect, m_rotation, &m_origin,
                 SDL_FLIP_NONE);
}
