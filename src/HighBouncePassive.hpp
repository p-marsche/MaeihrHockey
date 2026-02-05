#pragma once

#include "IPlayerPassiveComponent.hpp"

namespace mmt_gd
{
class HighBouncePassive final : public IPlayerPassiveComponent
{
public:
    HighBouncePassive(GameObject& go);

    void virtual apply();
    void virtual revert();

    float m_bounceFactor;
    bool  m_bounced;
};
}

