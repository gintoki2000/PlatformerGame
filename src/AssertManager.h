#ifndef ASSERT_MANAGER_H
#define ASSERT_MANAGER_H
#include "SDL.h"
#include "SDL_assert.h"
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
	virtual void free(void* resource) = 0;
};
class TextureHandler : public IResourceHandler
{
  public:
    void* load(const std::string& filename) override;

	void free(void* resource) override;
};
class AssertFactory
{
  public:
    AssertFactory(IResourceHandler* handler);
    ~AssertFactory();

    bool isLoaded(const std::string& filename)
    {
        return m_asserts.find(filename) != std::end(m_asserts);
    }

    template <typename T> T* getAssert(const std::string& filename)
    {
        auto assertPos = m_asserts.find(filename);
        if (assertPos == std::end(m_asserts))
        {
            T* res = (T*)m_handler->load(filename);
			SDL_assert(res != nullptr);
            m_asserts.insert(std::make_pair(filename, res));
            return res;
        }
        else
        {
            return (T*)assertPos->second;
        }
    }

  private:
    IResourceHandler* m_handler;
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
        auto factoryPos = m_factories.find(typeIndex);
        SDL_assert(factoryPos != std::end(m_factories));
        auto factory = factoryPos->second;
        return factory->isLoaded(filename);
    }

    template <typename T> void registerFactory(AssertFactory* factory)
    {
        auto typeIndex = std::type_index(typeid(T));
        auto factoryPos = m_factories.find(typeIndex);
        if (factoryPos == std::end(m_factories))
        {
            m_factories.insert(std::make_pair(typeIndex, factory));
        }
    }
    template <typename T> T* getAssert(const std::string& filename)
    {

        auto typeIndex = std::type_index(typeid(T));
        auto factoryPos = m_factories.find(typeIndex);
        SDL_assert(factoryPos != std::end(m_factories));
        auto factory = factoryPos->second;
        return factory->getAssert<T>(filename);
    }

  private:
    std::map<std::type_index, AssertFactory*> m_factories;
};

#endif // ASSERT_MANAGER_H
