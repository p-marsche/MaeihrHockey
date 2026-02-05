#include "stdafx.h"

#include "HighBouncePassive.hpp"
#include "GameObject.hpp"
#include "ColliderComponent.hpp"

namespace mmt_gd
{
float constexpr BASE_FACTOR = 1.5f;

HighBouncePassive::HighBouncePassive(GameObject& go) 
    : IPlayerPassiveComponent(go), 
    m_bounceFactor(BASE_FACTOR),
    m_bounced(false)
{
}

void HighBouncePassive::apply()
{
    auto bounce = m_gameObject.getComponent<ColliderComponent>()->getFixture()->GetRestitution();
    bounce *= m_bounceFactor;
    m_gameObject.getComponent<ColliderComponent>()->getFixture()->SetRestitution(bounce);
    m_bounced = true;
}

void HighBouncePassive::revert()
{
    if (!m_bounced)
        return;

    auto bounce = m_gameObject.getComponent<ColliderComponent>()->getFixture()->GetRestitution();
    bounce /= m_bounceFactor;
    m_gameObject.getComponent<ColliderComponent>()->getFixture()->SetRestitution(bounce);
    m_bounced = false;
}


}