#pragma once

#include "IPlayerPassiveComponent.hpp"

namespace mmt_gd
{
class HighDensityPassive final : public IPlayerPassiveComponent
{
public:
    HighDensityPassive(GameObject& go);

    void virtual apply();
    void virtual revert();

    float m_densityFactor;
};
}

