#include "stdafx.h"
#include "GameObject.hpp"
#include "InputManager.hpp"

#include "PlayerMoveComponent.hpp"
#include "PhysicsManager.hpp"

namespace mmt_gd
{
float constexpr SLOW_FACTOR = 3.f;

PlayerMoveComponent::PlayerMoveComponent(GameObject& gameObject, RigidBodyComponent& rigidBody, const int playerIndex) :
IComponent(gameObject),
m_playerIndex(playerIndex),
m_rigidBody(rigidBody)
{
}

bool PlayerMoveComponent::init()
{
    return true;
}

void PlayerMoveComponent::update(const float deltaTime)
{
    const auto   speed = 2'000.0F; // pixels/second
    sf::Vector2f translation{};
    if (InputManager::getInstance().isActionPressed("right", m_playerIndex))
    {
        translation.x += speed * deltaTime;
    }
    if (InputManager::getInstance().isActionPressed("left", m_playerIndex))
    {
        translation.x -= speed * deltaTime;
    }
    if (InputManager::getInstance().isActionPressed("up", m_playerIndex))
    {
        translation.y -= speed * deltaTime;
    }
    if (InputManager::getInstance().isActionPressed("down", m_playerIndex))
    {
        translation.y += speed * deltaTime;
    }

    if (translation.x == 0 && translation.y == 0)
        return;

    auto currVel = PhysicsManager::b2s(m_rigidBody.getB2Body()->GetLinearVelocity());
    auto temp    = sf::Vector2f();
    temp.x       = (currVel.x * translation.x > 0) ? 0 : (SLOW_FACTOR * currVel.x * deltaTime);
    temp.y       = (currVel.y * translation.y > 0) ? 0 : (SLOW_FACTOR * currVel.y * deltaTime);
    m_rigidBody.addVelocity(translation + temp);
}
} // namespace mmt_gd
