#include "stdafx.h"

#include "GameObject.hpp"
#include "InputManager.hpp"
#include "PhysicsManager.hpp"
#include "PlayerAbilityComponent.hpp"

namespace mmt_gd
{
PlayerAbilityComponent::PlayerAbilityComponent(GameObject& gameObject, RigidBodyComponent& rigidBody, const int playerIndex) :
IComponent(gameObject),
m_playerIndex(playerIndex),
m_rigidBody(rigidBody),
m_cdTimer(0.f)
{
}

bool PlayerAbilityComponent::init()
{
    return true;
}

} // namespace mmt_gd
