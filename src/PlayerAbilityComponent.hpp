#pragma once

#include "IComponent.hpp"
#include "RigidBodyComponent.hpp"

namespace mmt_gd
{

class PlayerAbilityComponent : public IComponent
{
public:
    using ptr = std::shared_ptr<PlayerAbilityComponent>;

    PlayerAbilityComponent(GameObject& gameObject, RigidBodyComponent& rigidBody, int playerIndex);

    bool init() override;
    void update(float deltaTime) override
    {
        return;
    }

    void virtual activate()
    {
        return;
    };

    void virtual updateInactive(float deltaTime)
    {
        return;
    }

protected:
    int   m_playerIndex;
    float m_cdTimer;

    RigidBodyComponent& m_rigidBody;
};
} // namespace mmt_gd

