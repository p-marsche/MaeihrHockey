#pragma once

#include "IComponent.hpp"
#include "RigidBodyComponent.hpp"

namespace mmt_gd
{

class IPlayerAbilityComponent : public IComponent
{
public:
    using ptr = std::shared_ptr<IPlayerAbilityComponent>;

    IPlayerAbilityComponent(GameObject& gameObject, RigidBodyComponent& rigidBody, int playerIndex, int cooldown, sf::Shader* cdShader)
        : IComponent(gameObject)
        , m_playerIndex(playerIndex)
        , m_rigidBody(rigidBody)
        , m_cdTimer(0.f)
        , m_cooldown(cooldown)
        , m_cdShader(cdShader)
    {
    }

    bool virtual init() override
    {
        return true;
    }
    void virtual update(float deltaTime) override
    {
        return;
    }
    void virtual updateInactive(float deltaTime)
    {
        return;
    }

    void virtual activate()
    {
        return;
    };

    // start at 0, is 1 if ability is ready
    // optimised for cd-shader-usage
    float getCooldownProgress() const
    {
        return (1 - (m_cdTimer/m_cooldown));
    }

    sf::Shader* getCdShader()
    {
        return m_cdShader;
    }

protected:
    int   m_playerIndex;
    float m_cdTimer, m_cooldown;
    sf::Shader* m_cdShader;

    RigidBodyComponent& m_rigidBody;
};
} // namespace mmt_gd

