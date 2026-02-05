#pragma once

#include "IComponent.hpp"
#include "RigidBodyComponent.hpp"

namespace mmt_gd
{

class IPlayerAbilityComponent : public IComponent
{
public:
    using ptr = std::shared_ptr<IPlayerAbilityComponent>;

    IPlayerAbilityComponent(GameObject& gameObject, RigidBodyComponent& rigidBody, int playerIndex)
        : IComponent(gameObject)
        , m_playerIndex(playerIndex)
        , m_rigidBody(rigidBody)
        , m_cdTimer(0.f)
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

protected:
    int   m_playerIndex;
    float m_cdTimer;

    RigidBodyComponent& m_rigidBody;
};
} // namespace mmt_gd

