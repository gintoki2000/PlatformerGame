#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H
#include "GameObject.h"
#include "tmxlite/Object.hpp"
#include <functional>
#include <map>
#include <string>
class ObjectFactory
{
  public:
    typedef std::function<GameObject*(const tmx::Object&)> CreateFunc;

    void Resigter(const std::string& type, CreateFunc createFunc);

    GameObject* CreateObject(const std::string& type, const tmx::Object& objectData);

	static ObjectFactory* GetInstance();

  private:
    ObjectFactory();
    std::map<std::string, CreateFunc> m_createFuncs;
};
#endif // OBJECT_FACTORY_H
