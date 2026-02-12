#include "stdafx.h"

#include "IPlayerPassiveComponent.hpp"
#include "GameObject.hpp"
#include "NoBouncePassive.hpp"
#include "RigidBodyComponent.hpp"

namespace mmt_gd
{
float constexpr BASE_FACTOR = 0.0f;
float constexpr EXTREME_DAMPENING = 1000.0f;

    NoBouncePassive::NoBouncePassive(GameObject& go) 
    : IPlayerPassiveComponent(go, PaddlePassive::TRAP)
    , m_bounceFactor(BASE_FACTOR)
    {
    }
    
    void NoBouncePassive::update(float deltaTime)
    {
        if (m_enabled)
        {
            auto body1 = m_gameObject.getComponent<RigidBodyComponent>()->getB2Body();
            body1->SetLinearDamping(EXTREME_DAMPENING);
        }
    }

    void NoBouncePassive::apply(b2Contact& contact)
    {
        if (m_enabled)
        {
            contact.SetRestitution(m_bounceFactor);
        }
    }
} 