#include "Background.h"
#include "AssertManager.h"
#include "Level.h"
#include "Locator.h"
#include "TextureRegion.h"
#include "tmxlite/ImageLayer.hpp"

Background::Background(SDL_Texture* texture, float parallax) :
    ImageLayer(texture), m_parallax(parallax)
{
}

void Background::update(float)
{
    float cameraPositionX = getManager()->getCamera().getViewport().left();
    setPositionX(m_initialPositionX + cameraPositionX * m_parallax);
}

bool Background::parse(const tmx::ImageLayer& imageLayerData)
{
    setPositionX(imageLayerData.getOffset().x);
    setPositionY(imageLayerData.getOffset().y);
    setIsVisible(imageLayerData.getVisible());

    TextureManager& tmgr = Locator::getTextureManager();
    if (!tmgr.load(imageLayerData.getImagePath()))
    {
        SDL_Log("Failed to load image!");
        return false;
    }
    SDL_Texture* texture = tmgr.get(imageLayerData.getImagePath());
    setImage(TextureRegion(texture));
    for (const auto& prop : imageLayerData.getProperties())
    {
        if (prop.getName() == "parallax")
        {
            m_parallax = prop.getFloatValue();
        }
    }
    return true;
}
