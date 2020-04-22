#include "FireCastParticle.h"
#include "AssertManager.h"
#include "Camera.h"
#include "Level.h"
#include "SpriteSheet.h"
#include "Game.h"
void FireCastParticle::init(const Vec2 &position)
{
    m_timer = 0.f;
    m_positionX = position.x;
    m_positionY = position.y;
}

FireCastParticle::FireCastParticle()
{
    SDL_Texture* texture = Game::getInstance()->textureMGR().getTexture(TextureManager::TEXTURE_FIRE_EXPLOSION);
    m_spriteSheet.init(texture, 96, 96);
    m_animation = new Animation(&m_spriteSheet, 1.f / 40.f);
}

void FireCastParticle::tick(float deltaTime)
{
	m_timer += deltaTime;
	if (m_animation->isFinished(m_timer))
	{
		setNeedToRemove(true);
		return;
	}
}

void FireCastParticle::cleanup() { setNeedToRemove(false); }


void FireCastParticle::paint()
{
	const Sprite& sprite = *(m_animation->getCurrentSprite(m_timer));
	
	const Camera& camera = getLayerManager()->getCamera();

	const SDL_Rect& viewport = camera.getViewport();

	SDL_Rect dstrect;
	dstrect.x = m_positionX - sprite.getWidth() / 2 - viewport.x;
	dstrect.y = m_positionY - sprite.getHeight() / 2 - viewport.y;
	dstrect.w = sprite.getWidth();
	dstrect.h = sprite.getHeight();	
	
	sprite.draw(Game::getInstance()->renderer(), &dstrect);
}
