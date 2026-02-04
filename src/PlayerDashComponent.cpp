#include "stdafx.h"

#include "GameObject.hpp"
#include "InputManager.hpp"
#include "PlayerDashComponent.hpp"
#include "PhysicsManager.hpp"

namespace mmt_gd
{
float constexpr BASE_COOLDOWN = 3.f;
float constexpr BASE_DASH_FACTOR = 3.f;

PlayerDashComponent::PlayerDashComponent(GameObject& gameObject, RigidBodyComponent& rigidBody, const int playerIndex) :
IComponent(gameObject),
m_playerIndex(playerIndex),
m_rigidBody(rigidBody),
m_cdTimer(0.f),
m_cooldown(BASE_COOLDOWN),
m_dashFactor(BASE_DASH_FACTOR)
{
}

bool PlayerDashComponent::init()
{
    return true;
}

void PlayerDashComponent::update(const float deltaTime)
{
    if (m_cdTimer > 0.f)
    {
        m_cdTimer -= deltaTime;
        return;
    }
    if (InputManager::getInstance().isKeyDown("dash", m_playerIndex))
    {
        auto sfVec = PhysicsManager::b2s(m_rigidBody.getB2Body()->GetLinearVelocity());
        auto dashVel = sf::Vector2f(sfVec.x * m_dashFactor, sfVec.y * m_dashFactor);
        m_rigidBody.addVelocity(dashVel);
        m_cdTimer = m_cooldown;
    }
}
} // namespace mmt_gd
