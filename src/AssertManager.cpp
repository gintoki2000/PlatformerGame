#include "AssertManager.h"
#include "Locator.h"
#include "SDL_image.h"
#include "SDL_render.h"
void* TextureHandler::load(const std::string& filename)
{
    return IMG_LoadTexture(Locator::getRenderer(), filename.c_str());
}

void TextureHandler::free(void *resource)
{
	SDL_DestroyTexture((SDL_Texture*) resource);
}

AssertFactory::AssertFactory(IResourceHandler* handler):
	m_handler(handler)
{
}

AssertFactory::~AssertFactory()
{
	for (const auto& p : m_asserts)
	{
		m_handler->free(p.second);
	}
	
	m_asserts.clear();
	delete m_handler;
}

AssertManager::AssertManager()
{	
	registerFactory<SDL_Texture>(new AssertFactory(new TextureHandler));
}

AssertManager::~AssertManager()
{
	for (const auto& p : m_factories)
	{
		delete p.second;
	}
	m_factories.clear();
}
