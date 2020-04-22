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
    Background() { m_texture = nullptr; }
    float m_parallax;
    float m_initialPositionX;

    SDL_Texture* m_texture;

    friend class Level;
};
#endif // BACKGROUND_H
