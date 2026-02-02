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
void ObjectFactory::propertyMissingNotice(std::string objName, std::string property)
{
    std::clog << "ERROR: tson::Object " << objName << " does not contain property \"" << property
              << "\". Return value is default value.";
}
} // namespace mmt_gd
