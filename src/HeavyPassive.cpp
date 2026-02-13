//This code was made for the Multimedia Project 2a for the Multimedia Technology class at Fachhochschule Salzburg, by Pascal Veselka & Pascal Marsche
#include "stdafx.h"

#include "IPlayerPassiveComponent.hpp"
#include "GameObject.hpp"
#include "HeavyPassive.hpp"
#include "RigidBodyComponent.hpp"

namespace mmt_gd
{
float constexpr EXTREME_LIN_DAMPENING = 1000.0f;

HeavyPassive::HeavyPassive(GameObject& go) :
IPlayerPassiveComponent(go, PaddlePassive::HEAVY)
{
}

void HeavyPassive::update(float deltaTime)
{
    if (m_enabled)
    {
        auto body = m_gameObject.getComponent<RigidBodyComponent>()->getB2Body();
        body->SetLinearDamping(EXTREME_LIN_DAMPENING);
    }
}
}; // namespace mmt_gd