#ifndef BACKGROUND_H
#define BACKGROUND_H
#include "ImageLayer.h"
#include "Layer.h"
namespace tmx
{
class ImageLayer;
}
class Background : public ImageLayer
{
  public:
    static Background* create(const tmx::ImageLayer& data);

	~Background();

    bool init(const tmx::ImageLayer& imageLayerData);
    void update(float deltaTime) override;

  private:
    Background(); 
    float m_parallax;
    float m_initialPositionX;

    SDL_Texture* m_texture;
};
#endif // BACKGROUND_H
