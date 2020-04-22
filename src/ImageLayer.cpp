#include "ImageLayer.h"
#include "Game.h"
#include "LayerManager.h"
#include "SDL_rect.h"
#include "SDL_render.h"

ImageLayer::ImageLayer(SDL_Texture* texture) : m_image(texture) {}

ImageLayer::ImageLayer(const TextureRegion& image) : m_image(image) {}

const TextureRegion& ImageLayer::getImage() const { return m_image; }
void ImageLayer::setImage(const TextureRegion& image) { m_image = image; }

void ImageLayer::render()
{
    SDL_Renderer*   renderer = Game::getInstance()->renderer();
    const SDL_Rect& viewport = getManager()->getCamera().getViewport();
    SDL_Rect        dstrect;
    dstrect.x = getPositionX() - viewport.x;
    dstrect.y = getPositionY() - viewport.y;
    dstrect.w = m_image.getWidth();
    dstrect.h = m_image.getHeight();
    m_image.draw(renderer, &dstrect);
}

ImageLayer::ImageLayer() {}
