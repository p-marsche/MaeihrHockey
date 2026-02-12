#include "stdafx.h"

#include "IPlayerPassiveComponent.hpp"
#include "GameObject.hpp"
#include "NoBouncePassive.hpp"
#include "RigidBodyComponent.hpp"

namespace mmt_gd
{
float constexpr BASE_FACTOR = 0.0f;

NoBouncePassive::NoBouncePassive(GameObject& go) :
IPlayerPassiveComponent(go, PaddlePassive::TRAP),
m_bounceFactor(BASE_FACTOR)
{
}

void NoBouncePassive::apply(b2Contact& contact)
{
    if (m_enabled)
    {
        contact.SetRestitution(m_bounceFactor);
    }
}
}; // namespace mmt_gd