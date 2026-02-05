#pragma once

#include "IComponent.hpp"
#include "RigidBodyComponent.hpp"
#include "PlayerAbilityComponent.hpp"

namespace mmt_gd
{

class PlayerDashComponent final : public PlayerAbilityComponent
{
public:
    using ptr = std::shared_ptr<PlayerDashComponent>;

    PlayerDashComponent(GameObject& gameObject, RigidBodyComponent& rigidBody, int playerIndex);

    bool init() override;
    void update(float deltaTime) override;
    void updateInactive(float deltaTime) override;

private:
    int m_cooldown;
    float m_dashFactor;
};
} // namespace mmt_gd