#ifndef IMAGE_LAYER_H
#define IMAGE_LAYER_H
#include "PositionalLayer.h"
#include "TextureRegion.h"
class ImageLayer : public PositionalLayer
{
  public:
    ImageLayer(SDL_Texture* texture);
    ImageLayer(const TextureRegion& image);

    const TextureRegion& getImage() const;
    void                 setImage(const TextureRegion& image);
    void                 render() override;

  private:
    TextureRegion m_image;

  protected:
    ImageLayer();
};
#endif // IMAGE_LAYER_H
