//This code was made for the Multimedia Project 2a for the Multimedia Technology class at Fachhochschule Salzburg, by Pascal Veselka & Pascal Marsche
#include "stdafx.h"

#include "HighBouncePassive.hpp"
#include "GameObject.hpp"
#include "ColliderComponent.hpp"

namespace mmt_gd
{
float constexpr BASE_FACTOR = 3.f;

HighBouncePassive::HighBouncePassive(GameObject& go) 
    : IPlayerPassiveComponent(go, PaddlePassive::BOUNCY)
    , m_bounceFactor(BASE_FACTOR)
{
}

void HighBouncePassive::apply(b2Contact& contact)
{
    if (m_enabled)
    {
        contact.SetRestitution(m_bounceFactor);
        contact.SetRestitutionThreshold(0.1f);
    }
}
} // namespace mmt_gd