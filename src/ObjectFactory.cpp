#include "stdafx.h"
#include "ColliderComponent.hpp"
#include "GameObjectEvents.hpp"
#include "PhysicsManager.hpp"
#include "PlayerMoveComponent.hpp"
#include "PlayerShootComponent.hpp"
#include "SpriteRenderComponent.hpp"
#include "Tileson.hpp"
#include <iostream>

#include "ObjectFactory.hpp"


namespace mmt_gd
{
float ObjectFactory::getDensity(tson::Object& obj)
{
    return getProperty<float>(obj, "Density");
}

int ObjectFactory::getPlayerIndex(tson::Object& obj)
{
    return getProperty<int>(obj, "PlayerIndex");
}

std::string ObjectFactory::getShape(tson::Object& obj)
{
    return getProperty<std::string>(obj, "Shape");
}

std::string ObjectFactory::getTexture(tson::Object& obj)
{
    return getProperty<std::string>(obj, "Texture");
}

bool ObjectFactory::isSensor(tson::Object& obj)
{
    return getProperty<bool>(obj, "Sensor");
}

void ObjectFactory::propertyMissingNotice(std::string objName, std::string property)
{
    std::clog << "ERROR: tson::Object " << objName << " does not contain property \"" << property
              << "\". Return value is default value.";
}
} // namespace mmt_gd
