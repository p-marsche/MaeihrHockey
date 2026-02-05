#include "stdafx.h"

#include "GameObject.hpp"
#include "InputManager.hpp"
#include "PhysicsManager.hpp"
#include "PlayerEnlargeComponent.hpp"

    namespace mmt_gd
{
    float constexpr BASE_COOLDOWN    = 3.f;
    float constexpr BASE_ENLARGE_FACTOR = 1.5f;
    float constexpr BASE_DURATION       = 1.5f;

    PlayerEnlargeComponent::PlayerEnlargeComponent(GameObject & gameObject, RigidBodyComponent & rigidBody, 
        ColliderComponent& coll, SpriteRenderComponent& sprite, const int playerIndex) :
    PlayerAbilityComponent(gameObject, rigidBody, playerIndex),
    m_collider(coll),
    m_sprite(sprite),
    m_cooldown(BASE_COOLDOWN),
    m_enlargeFactor(BASE_ENLARGE_FACTOR),
    m_durationTotal(BASE_DURATION),
    m_durationTimer(0.f),
    m_endDuration(false)
    {
    }

    bool PlayerEnlargeComponent::init()
    {
        return true;
    }

    void PlayerEnlargeComponent::update(const float deltaTime)
    { 
        if (m_durationTimer > 0.f)
        {
            m_durationTimer -= deltaTime;
            m_cdTimer -= deltaTime;
            return;
        }
        else if (m_cdTimer > 0.f && !m_endDuration)
        {
            changeSize(false);
            m_endDuration = true;
        }

        if (m_cdTimer > 0.f)
            return;

        if (InputManager::getInstance().isActionJustPressed("ability", m_playerIndex))
        {
            std::cout << "dash" << std::endl;
            changeSize(true);
            m_cdTimer = m_cooldown + m_durationTotal;
            m_durationTimer = m_durationTotal;
            m_endDuration   = false;
        }
    }

    void PlayerEnlargeComponent::updateInactive(float deltaTime)
    {
        if (m_durationTimer > 0.f)
        {
            m_durationTimer -= deltaTime;
            m_cdTimer -= deltaTime;
            return;
        }
        else if (m_cdTimer > 0.f && !m_endDuration)
        {
            changeSize(false);
            m_endDuration = true;
        }
    }

    void PlayerEnlargeComponent::changeSize(bool enlarge)
    {
        auto shape = m_collider.getFixture()->GetShape();
        auto& scale = m_sprite.getSprite().getScale();
        auto  newScale = sf::Vector2f(0.f, 0.f);
        if (enlarge)
        {
            shape->m_radius *= m_enlargeFactor;
            newScale = sf::Vector2f(scale.x * m_enlargeFactor, scale.y * m_enlargeFactor);
        }
        else
        {
            shape->m_radius /= m_enlargeFactor;
            newScale = sf::Vector2f(scale.x / m_enlargeFactor, scale.y / m_enlargeFactor);
        }
        m_sprite.setScale(newScale);
    }
    } // namespace mmt_gd
