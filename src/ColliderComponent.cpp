#include "stdafx.h"

#include "ColliderComponent.hpp"

#include "DebugDraw.hpp"
#include "EventBus.hpp"
#include "GameObject.hpp"
#include "PhysicsComponentEvents.hpp"
#include "PhysicsManager.hpp"

namespace mmt_gd
{
ColliderComponent::ColliderComponent(GameObject& gameObject, RigidBodyComponent& body, b2FixtureDef& def) :
IComponent(gameObject),
m_body(body),
m_fixture(nullptr)
{
    def.userData.pointer = reinterpret_cast<uintptr_t>(this);
    m_fixture            = m_body.getB2Body()->CreateFixture(&def);
    delete def.shape;
}

void ColliderComponent::update(float deltaTime)
{
    auto shape = m_fixture->GetShape();
    if (shape->m_type == b2Shape::Type::e_circle)
    {
        auto circle = static_cast<const b2CircleShape*>(shape);
        auto radius = (PhysicsManager::b2s(b2Vec2(circle->m_radius, 0))).x;
        auto pos    = sf::Vector2f(m_gameObject.getPosition().x, m_gameObject.getPosition().y);
        DebugDraw::getInstance().drawCircle(pos, radius, sf::Color::Red);
    }
    else
    {
        auto box = m_fixture->GetAABB(0);
        auto ex  = box.GetExtents();
        auto sfVec = PhysicsManager::b2s(ex);
        auto pos   = m_gameObject.getPosition();
        DebugDraw::getInstance().drawRectangle(pos, {sfVec.x * 2, sfVec.y * 2}, sf::Color::Red);
    }
}

void ColliderComponent::registerOnCollisionFunction(const OnCollisionFunction& func)
{
    m_onCollisionFunctions.push_back(func);
}

void ColliderComponent::onCollision(ColliderComponent& collider)
{
    for (const auto& f : m_onCollisionFunctions)
    {
        f(*this, collider);
    }
}
} // namespace mmt_gd
