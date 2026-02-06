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
class TsonPropertyReader
{
public:
    static float getDensity(tson::Object& obj);
    static int   getPlayerIndex(tson::Object& obj);
    static std::string getShape(tson::Object& obj);
    static std::string getTexture(tson::Object& obj);
    static bool        isSensor(tson::Object& obj);
    static std::string getWallSide(tson::Object& obj);
    static void TsonPropertyReader::propertyMissingNotice(std::string objName, std::string property);

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
