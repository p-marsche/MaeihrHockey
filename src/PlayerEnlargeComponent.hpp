#pragma once

#include "IComponent.hpp"
#include "RigidBodyComponent.hpp"
#include "ColliderComponent.hpp"
#include "SpriteRenderComponent.hpp"

namespace mmt_gd
{

class PlayerEnlargeComponent : public IComponent
{
public:
    using ptr = std::shared_ptr<PlayerEnlargeComponent>;

    PlayerEnlargeComponent(GameObject& gameObject, RigidBodyComponent& rigidBody, 
        ColliderComponent& coll, SpriteRenderComponent& sprite, int playerIndex);

    bool init() override;
    void update(float deltaTime) override;
    void changeSize(bool enlareg);

private:
    int   m_playerIndex, m_cooldown;
    float m_cdTimer, m_enlargeFactor, m_durationTimer, m_durationTotal;
    bool  m_endDuration;

    RigidBodyComponent& m_rigidBody;
    ColliderComponent&  m_collider;
    SpriteRenderComponent& m_sprite;
};
} // namespace mmt_gd

