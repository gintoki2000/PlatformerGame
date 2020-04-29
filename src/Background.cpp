#include "Background.h"
#include "AssertManager.h"
#include "Game.h"
#include "Level.h"
#include "SDL_image.h"
#include "SDL_log.h"
#include "SDL_render.h"
#include "TextureRegion.h"
#include "tmxlite/ImageLayer.hpp"

Background::Background() :
    m_parallax(0.f), m_initialPositionX(0.f), m_texture(nullptr)
{
}

Background* Background::create(const tmx::ImageLayer& data)
{
    Background* ret = new Background;
    if (ret->init(data))
    {
        return ret;
    }
    delete ret;
    return nullptr;
}

Background::~Background() { SDL_DestroyTexture(m_texture); }

bool Background::init(const tmx::ImageLayer& imageLayerData)
{
    setPositionX(imageLayerData.getOffset().x);
    setPositionY(imageLayerData.getOffset().y);
    setIsVisible(imageLayerData.getVisible());

    const char*  path      = imageLayerData.getImagePath().c_str();
    SDL_Texture* m_texture = IMG_LoadTexture(GAME->renderer(), path);

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
void Background::update(float)
{
    float cameraPositionX = getManager()->getCamera().getViewport().left();
    setPositionX(m_initialPositionX + cameraPositionX * m_parallax);
}
