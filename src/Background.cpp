#include "Background.h"

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

Background* Background::Create(const tmx::ImageLayer& data)
{
    Background* ret = new Background;
    if (ret->Init(data))
    {
        return ret;
    }
    delete ret;
    return nullptr;
}

Background::~Background() { SDL_DestroyTexture(m_texture); }

bool Background::Init(const tmx::ImageLayer& imageLayerData)
{
    SetPositionX(imageLayerData.getOffset().x);
    SetPositionY(imageLayerData.getOffset().y);
    SetIsVisible(imageLayerData.getVisible());

    const char*  path      = imageLayerData.getImagePath().c_str();
    SDL_Texture* m_texture = IMG_LoadTexture(GAME->GetRenderer(), path);

    if (m_texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s",
                     imageLayerData.getImagePath().c_str());
        return false;
    }

    SetImage(m_texture);

    m_initialPositionX = GetPositionX();

    for (const auto& prop : imageLayerData.getProperties())
    {
        if (prop.getName() == "parallax")
        {
            m_parallax = prop.getFloatValue();
        }
    }
    return true;
}
void Background::Update(float)
{
    float cameraPositionX = GetScene()->GetCamera().GetViewport().Left();
    SetPositionX(m_initialPositionX + cameraPositionX * m_parallax);
}
