#include "stdafx.h"

#include "TsonPropertyReader.hpp"

#include "ColliderComponent.hpp"
#include "GameObjectEvents.hpp"
#include "PhysicsManager.hpp"
#include "PlayerMoveComponent.hpp"
#include "PlayerShootComponent.hpp"
#include "SpriteRenderComponent.hpp"
#include "Tileson.hpp"

#include <iostream>


namespace mmt_gd
{
float TsonPropertyReader::getDensity(tson::Object& obj)
{
    return getProperty<float>(obj, "Density");
}

int TsonPropertyReader::getPlayerIndex(tson::Object& obj)
{
    return getProperty<int>(obj, "PlayerIndex");
}

std::string TsonPropertyReader::getShape(tson::Object& obj)
{
    return getProperty<std::string>(obj, "Shape");
}

std::string TsonPropertyReader::getTexture(tson::Object& obj)
{
    return getProperty<std::string>(obj, "Texture");
}

bool TsonPropertyReader::isSensor(tson::Object& obj)
{
    return getProperty<bool>(obj, "Sensor");
}

void TsonPropertyReader::propertyMissingNotice(std::string objName, std::string property)
{
    std::clog << "ERROR: tson::Object " << objName << " does not contain property \"" << property
              << "\". Return value is default value.";
}
} // namespace mmt_gd
