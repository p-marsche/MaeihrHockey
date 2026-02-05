#include "stdafx.h"
#include "GameObject.hpp"
#include "InputManager.hpp"
#include "VectorAlgebra2D.h"

#include "PlayerMoveComponent.hpp"
#include "PhysicsManager.hpp"

namespace mmt_gd
{
float constexpr SLOW_FACTOR = 3.f;
float constexpr MAX_VELOCITY = 1000.F;

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

    auto currVel = PhysicsManager::b2s(m_rigidBody.getB2Body()->GetLinearVelocity());
    auto velMag  = MathUtil::length<float>(currVel);
    if (velMag > MAX_VELOCITY)
        MathUtil::setLength(currVel, MAX_VELOCITY);

    auto temp    = sf::Vector2f();
    temp.x       = (currVel.x * translation.x > 0) ? 0 : (currVel.x);
    temp.y       = (currVel.y * translation.y > 0) ? 0 : (currVel.y);
    m_rigidBody.addVelocity(translation - temp * SLOW_FACTOR * deltaTime);
}
} // namespace mmt_gd
