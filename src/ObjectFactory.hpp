#pragma once
#include "GameObject.hpp"
#include "SpriteManager.hpp"
#include "Tileson.hpp"

namespace tson
{
class Object;
}

namespace mmt_gd
{
class ObjectFactory
{
public:
    static void ObjectFactory::propertyMissingNotice(std::string objName, std::string property);

    template <typename T>
    static T getProperty(tson::Object& obj, std::string propertyName)
    {
        tson::PropertyCollection& properties = obj.getProperties();
        if (!properties.hasProperty(propertyName))
            propertyMissingNotice(obj.getName(), propertyName);
        return obj.getProperties().getValue<T>(propertyName);
    }
};
} //namespace mmt_gd
