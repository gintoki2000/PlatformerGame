#include "MyObjectLayer.h"
#include "tmxlite/ObjectGroup.hpp"
#include "ObjectFactory.h"
MyObjectLayer* MyObjectLayer::create(const tmx::ObjectGroup &data)
{
	MyObjectLayer* ret = new MyObjectLayer;
	if (ret->init(data))
	{
		return ret;
	}
	delete ret;
	return nullptr;
}

bool MyObjectLayer::init(const tmx::ObjectGroup& data)
{
		
    ObjectFactory& factory = *ObjectFactory::getInstance();
    for (const auto& objectData : data.getObjects())
    {
        GameObject* object =
            factory.createObject(objectData.getType(), objectData);
        if (object != nullptr)
        {
            addObject(object);
        }
    }
    setName(data.getName());
	return true;
}
