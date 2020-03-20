#ifndef ASSERT_MANAGER_H
#define ASSERT_MANAGER_H
#include "SDL.h"
#include "SDL_assert.h"
#include "SDL_render.h"
#include <map>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <utility>
class IResourceHandler
{
  public:
    virtual ~IResourceHandler(){};
    virtual void* load(const std::string&) = 0;
    virtual void  free(void* resource) = 0;
};
class TextureHandler : public IResourceHandler
{
  public:
	TextureHandler(SDL_Renderer* renderer);
    void* load(const std::string& filename) override;
    void free(void* resource) override;
  private:
	SDL_Renderer* m_renderer;
};
class AssertFactory
{
  public:
    AssertFactory(IResourceHandler* handler);
    ~AssertFactory();

    bool isLoaded(const std::string& filename);
    bool loadAssert(const std::string& filename);

    template <typename T> T* getAssert(const std::string& filename)
    {
        auto findResult = m_asserts.find(filename);
        if (findResult != std::end(m_asserts))
        {
            return dynamic_cast<T*>(findResult->second);
        }
        return nullptr;
    }

  private:
    IResourceHandler*            m_handler;
    std::map<std::string, void*> m_asserts;
};
class AssertManager
{
  public:
    AssertManager();
    ~AssertManager();

    template <typename T> bool isLoaded(const std::string& filename)
    {
        auto typeIndex = std::type_index(typeid(T));
        auto findResult = m_factories.find(typeIndex);
        SDL_assert(findResult != std::end(m_factories));
        auto factory = findResult->second;
        return factory->isLoaded(filename);
    }

    template <typename T> void registerFactory(AssertFactory* factory)
    {
        auto typeIndex = std::type_index(typeid(T));
        auto findResult = m_factories.find(typeIndex);
        if (findResult == std::end(m_factories))
        {
            m_factories.insert(std::make_pair(typeIndex, factory));
        }
    }
    template <typename T> bool loasAssert(const std::string& filename)
    {
        auto typeIndex = std::type_index(typeid(T));
        auto findResult = m_factories.find(typeIndex);
        SDL_assert(findResult != std::end(m_factories));
        return findResult->second->loadAssert(filename);
    }
    template <typename T> T* getAssert(const std::string& filename)
    {

        auto typeIndex = std::type_index(typeid(T));
        auto findResult = m_factories.find(typeIndex);
        SDL_assert(findResult != std::end(m_factories));
        return findResult->second->getAssert<T>(filename);
    }

  private:
    std::map<std::type_index, AssertFactory*> m_factories;
};

#endif // ASSERT_MANAGER_H
