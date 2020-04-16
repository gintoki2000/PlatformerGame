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
    Background(SDL_Texture* texture, float parallax);

    bool parse(const tmx::ImageLayer& imageLayerData);
    void update(float deltaTime) override;

  private:
    Background() {}
    float m_parallax;
    float m_initialPositionX;

    friend class LevelParser;
};
#endif // BACKGROUND_H
