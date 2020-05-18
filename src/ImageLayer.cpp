#include "ImageLayer.h"
#include "Game.h"
#include "Scene.h"
#include "SDL_rect.h"
#include "SDL_render.h"

ImageLayer::ImageLayer(SDL_Texture* texture) : m_image(texture) {}

ImageLayer::ImageLayer(const TextureRegion& image) : m_image(image) {}

const TextureRegion& ImageLayer::GetImage() const { return m_image; }
void ImageLayer::SetImage(const TextureRegion& image) { m_image = image; }

void ImageLayer::Paint()
{
    SDL_Renderer*   renderer = Game::GetInstance()->GetRenderer();
    const SDL_Rect& viewport = GetScene()->GetCamera().GetViewport();
    SDL_Rect        dstrect;
    dstrect.x = GetPositionX() - viewport.x;
    dstrect.y = GetPositionY() - viewport.y;
    dstrect.w = m_image.GetWidth();
    dstrect.h = m_image.GetHeight();
    m_image.Draw(renderer, &dstrect);
}

ImageLayer::ImageLayer() {}
