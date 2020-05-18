#include "ObjectFactory.h"
#include "GameObject.h"
#include "SDL_log.h"
#include "tmxlite/Object.hpp"

ObjectFactory::ObjectFactory() {}

void ObjectFactory::Resigter(const std::string& type, CreateFunc createFunc)
{
    auto iter = m_createFuncs.find(type);
    if (iter != std::end(m_createFuncs))
    {
        return;
    }
    m_createFuncs.insert(std::make_pair(type, createFunc));
}

GameObject* ObjectFactory::CreateObject(const std::string& type,
                                        const tmx::Object& objectData)
{
    auto iter = m_createFuncs.find(type);
    if (iter == std::end(m_createFuncs))
    {
        SDL_Log("Could not create object of type %s: type not found!",
                type.c_str());
        return nullptr;
    }
    return iter->second(objectData);
}

ObjectFactory* ObjectFactory::GetInstance()
{
    static ObjectFactory instance;
    return &instance;
}
