#pragma once

#include "IComponent.hpp"
#include "IPlayerAbilityComponent.hpp"
#include "RigidBodyComponent.hpp"
#include "ColliderComponent.hpp"
#include "SpriteRenderComponent.hpp"

namespace mmt_gd
{

class PlayerEnlargeComponent final : public IPlayerAbilityComponent
{
public:
    using ptr = std::shared_ptr<PlayerEnlargeComponent>;

    PlayerEnlargeComponent(GameObject& gameObject, RigidBodyComponent& rigidBody, 
        ColliderComponent& coll, SpriteRenderComponent& sprite, int playerIndex, sf::Shader* cdShader);

    bool init() override;
    void update(float deltaTime) override;
    void changeSize(bool enlareg);
    void updateInactive(float deltaTime) override;

private:
    float m_enlargeFactor, m_durationTimer, m_durationTotal;
    bool  m_endDuration;

    ColliderComponent&  m_collider;
    SpriteRenderComponent& m_sprite;
};
} // namespace mmt_gd

