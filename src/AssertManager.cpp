#include "AssertManager.h"
#include "Locator.h"
#include "SDL_image.h"
#include "SDL_render.h"
TextureHandler::TextureHandler(SDL_Renderer* renderer) : m_renderer(renderer) {}
void* TextureHandler::load(const std::string& filename)
{
    return IMG_LoadTexture(m_renderer, filename.c_str());
}

void TextureHandler::free(void* resource)
{
    SDL_DestroyTexture((SDL_Texture*)resource);
}

AssertFactory::AssertFactory(IResourceHandler* handler) : m_handler(handler) {}

AssertFactory::~AssertFactory()
{
    for (const auto& p : m_asserts)
    {
        m_handler->free(p.second);
    }

    m_asserts.clear();
    delete m_handler;
}

bool AssertFactory::loadAssert(const std::string& filename)
{
    auto findResult = m_asserts.find(filename);

    if (findResult == std::end(m_asserts))
    {
        void* res = m_handler->load(filename);
        if (res == nullptr)
        {
            return false;
        }
        m_asserts.insert(std::make_pair(filename, res));
        return true;
    }
    else
    {
        return false;
    }
}
bool AssertFactory::isLoaded(const std::string& filename)
{
    return m_asserts.find(filename) != std::end(m_asserts);
}
AssertManager::AssertManager() {}

AssertManager::~AssertManager()
{
    for (const auto& p : m_factories)
    {
        delete p.second;
    }
    m_factories.clear();
}
