#include "stdafx.h"
#include "GameObject.hpp"
#include "InputManager.hpp"

#include "PlayerMoveComponent.hpp"

namespace mmt_gd
{
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
    if (InputManager::getInstance().isActionJustPressed("right", m_playerIndex))
    {
        translation.x += speed * deltaTime;
    }
    if (InputManager::getInstance().isActionJustPressed("left", m_playerIndex))
    {
        translation.x -= speed * deltaTime;
    }
    if (InputManager::getInstance().isActionJustPressed("up", m_playerIndex))
    {
        translation.y -= speed * deltaTime;
    }
    if (InputManager::getInstance().isActionJustPressed("down", m_playerIndex))
    {
        translation.y += speed * deltaTime;
    }

    m_rigidBody.addVelocity(translation);
}
} // namespace mmt_gd
