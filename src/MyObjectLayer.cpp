#include "MyObjectLayer.h"
#include "tmxlite/ObjectGroup.hpp"
#include "ObjectFactory.h"
MyObjectLayer* MyObjectLayer::Create(const tmx::ObjectGroup &data)
{
	MyObjectLayer* ret = new MyObjectLayer;
	if (ret->Init(data))
	{
		return ret;
	}
	delete ret;
	return nullptr;
}

bool MyObjectLayer::Init(const tmx::ObjectGroup& data)
{
		
    ObjectFactory& factory = *ObjectFactory::GetInstance();
    for (const auto& objectData : data.getObjects())
    {
        GameObject* object =
            factory.CreateObject(objectData.getType(), objectData);
        if (object != nullptr)
        {
            AddObject(object);
        }
    }
    SetName(data.getName());
	return true;
}
