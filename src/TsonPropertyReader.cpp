#include "stdafx.h"

#include "TsonPropertyReader.hpp"

#include "ColliderComponent.hpp"
#include "GameObjectEvents.hpp"
#include "PhysicsManager.hpp"
#include "PlayerMoveComponent.hpp"
#include "SpriteRenderComponent.hpp"
#include "Tileson.hpp"

#include <iostream>


namespace mmt_gd
{
std::string TsonPropertyReader::getName(tson::Object& obj)
{
    return obj.getName();
}

std::string TsonPropertyReader::getType(tson::Object& obj)
{
    return obj.getType();
}

sf::Vector2f TsonPropertyReader::getPosition(tson::Object& obj)
{
    return vec2s(obj.getPosition());
}

sf::Vector2f TsonPropertyReader::getSize(tson::Object& obj)
{
    return vec2s(obj.getSize());
}

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

std::string TsonPropertyReader::getWallSide(tson::Object& obj)
{
    return getProperty<std::string>(obj, "WallSide");
}

void TsonPropertyReader::propertyMissingNotice(std::string objName, std::string property)
{
    std::clog << "ERROR: tson::Object " << objName << " does not contain property \"" << property
              << "\". Return value is default value.";
}

sf::Vector2f TsonPropertyReader::vec2s(const tson::Vector2i& vec)
{
    return sf::Vector2f(vec.x, vec.y);
}
} // namespace mmt_gd
