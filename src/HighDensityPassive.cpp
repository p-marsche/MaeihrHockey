#include "stdafx.h"

#include "HighDensityPassive.hpp"
#include "GameObject.hpp"
#include "ColliderComponent.hpp"

namespace mmt_gd
{
float constexpr BASE_FACTOR = 5.f;

    HighDensityPassive::HighDensityPassive(GameObject& go) 
    : IPlayerPassiveComponent(go)
    , m_densityFactor(BASE_FACTOR)
    {
    }
    
    void HighDensityPassive::apply()
    {
        auto density = m_gameObject.getComponent<ColliderComponent>()->getFixture()->GetDensity();
        std::cout << m_gameObject.getId() << " | " << density << std::endl;
        density *= m_densityFactor;
        m_gameObject.getComponent<ColliderComponent>()->getFixture()->SetDensity(density);
        std::cout << m_gameObject.getId() << " | " << density << std::endl;
    }

    void HighDensityPassive::revert()
    {
        auto density = m_gameObject.getComponent<ColliderComponent>()->getFixture()->GetDensity();
        density /= m_densityFactor;
        m_gameObject.getComponent<ColliderComponent>()->getFixture()->SetDensity(density);
    }


}