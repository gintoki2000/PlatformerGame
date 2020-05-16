#ifndef IMAGE_LAYER_H
#define IMAGE_LAYER_H
#include "PositionalLayer.h"
#include "TextureRegion.h"
class ImageLayer : public PositionalLayer
{
  public:
    ImageLayer(SDL_Texture* texture);
    ImageLayer(const TextureRegion& image);

    const TextureRegion& GetImage() const;
    void                 SetImage(const TextureRegion& image);
    void                 Render() override;

  private:
    TextureRegion m_image;

  protected:
    ImageLayer();
};
#endif // IMAGE_LAYER_H
