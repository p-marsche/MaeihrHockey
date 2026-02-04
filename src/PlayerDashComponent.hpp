#pragma once

#include "IComponent.hpp"
#include "RigidBodyComponent.hpp"

namespace mmt_gd
{

class PlayerDashComponent : public IComponent
{
public:
    using ptr = std::shared_ptr<PlayerDashComponent>;

    PlayerDashComponent(GameObject& gameObject, RigidBodyComponent& rigidBody, int playerIndex);

    bool init() override;
    void update(float deltaTime) override;

private:
    int m_playerIndex, m_cooldown;
    float m_cdTimer, m_dashFactor;

    RigidBodyComponent& m_rigidBody;
};
} // namespace mmt_gd