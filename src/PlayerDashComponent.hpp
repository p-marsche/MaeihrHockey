//This code was made for the Multimedia Project 2a for the Multimedia Technology class at Fachhochschule Salzburg, by Pascal Veselka & Pascal Marsche
#pragma once

#include "IComponent.hpp"
#include "RigidBodyComponent.hpp"
#include "IPlayerAbilityComponent.hpp"

namespace mmt_gd
{

class PlayerDashComponent final : public IPlayerAbilityComponent
{
public:
    using ptr = std::shared_ptr<PlayerDashComponent>;

    PlayerDashComponent(GameObject& gameObject, RigidBodyComponent& rigidBody, int playerIndex, sf::Shader* cdShader);

    bool init() override;
    void update(float deltaTime) override;
    void updateInactive(float deltaTime) override;

private:
    float m_dashFactor;
};
} // namespace mmt_gd