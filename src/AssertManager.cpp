#include "AssertManager.h"
#include "SDL_image.h"

TextureManager::TextureManager(SDL_Renderer* renderer) : m_renderer(renderer) {}

TextureManager::~TextureManager() { unloadAll(); }

bool TextureManager::isLoaded(const std::string& filename)
{
    return m_textures.find(filename) != std::end(m_textures);
}

bool TextureManager::load(const std::string& filename)
{
    if (isLoaded(filename))
    {
        return true;
    }
    SDL_Texture* texture = IMG_LoadTexture(m_renderer, filename.c_str());
    if (texture == nullptr)
    {
        return false;
    }
    m_textures.insert(std::make_pair(filename, texture));
    return true;
}

SDL_Texture* TextureManager::get(const std::string& filename)
{
    auto findResult = m_textures.find(filename);
    if (findResult == std::end(m_textures))
    {
        return nullptr;
    }
    return findResult->second;
}

void TextureManager::unloadAll()
{
    for (const auto& p : m_textures)
    {
        SDL_DestroyTexture(p.second);
    }
    m_textures.clear();
}
