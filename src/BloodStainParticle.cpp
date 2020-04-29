#include "BloodStainParticle.h"
#include "Animation.h"
#include "AssertManager.h"
#include "Game.h"
#include "LayerManager.h"
#include "Utils.h"

BloodStainParticle::BloodStainParticle() : m_animation(nullptr) { preinit(); }

BloodStainParticle::~BloodStainParticle() { DELETE_NULL(m_animation); }

bool BloodStainParticle::preinit()
{
    TextureManager& tmgr    = Game::getInstance()->textureMGR();
    SDL_Texture*    texture = tmgr.getTexture(TextureManager::BLOOD_STAIN);

    m_spritesheet.init(texture, SPRITE_WIDTH, SPRITE_HEIGHT);
    m_animation = new Animation(&m_spritesheet, 0, 22, 1.f / 20.f);
    return true;
}

void BloodStainParticle::init(const Vec2& position)
{
    m_positionX = position.x;
    m_positionY = position.y;
    m_timer     = 0.f;
}

void BloodStainParticle::tick(float deltaTime)
{
    m_timer += deltaTime;
    if (m_animation->isFinished(m_timer))
    {
        remove();
    }
}

void BloodStainParticle::paint()
{

    const Sprite& sprite = *m_animation->getCurrentSprite(m_timer);

    SDL_Renderer*   renderer = Game::getInstance()->renderer();
    const SDL_Rect& viewport = getLayerManager()->getCamera().getViewport();

    SDL_Rect dstrect;
    dstrect.x = m_positionX - SPRITE_WIDTH / 2 - viewport.x;
    dstrect.y = m_positionY - SPRITE_HEIGHT / 2 - viewport.y;
    dstrect.w = sprite.getWidth();
    dstrect.h = sprite.getHeight();

    sprite.draw(renderer, &dstrect, 0.f, nullptr, SDL_FLIP_NONE);
}

void BloodStainParticle::cleanup() { getPool()->free(this); }
