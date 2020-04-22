#include "Background.h"
#include "AssertManager.h"
#include "Game.h"
#include "Level.h"
#include "SDL_image.h"
#include "SDL_log.h"
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

    SDL_Texture* m_texture = IMG_LoadTexture(
        Game::getInstance()->renderer(), imageLayerData.getImagePath().c_str());

    if (m_texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s",
                     imageLayerData.getImagePath().c_str());
        return false;
    }

	setImage(m_texture);

	m_initialPositionX = getPositionX();

    for (const auto& prop : imageLayerData.getProperties())
    {
        if (prop.getName() == "parallax")
        {
            m_parallax = prop.getFloatValue();
        }
    }
    return true;
}
