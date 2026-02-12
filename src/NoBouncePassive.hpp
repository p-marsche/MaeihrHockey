#pragma once

#include "IPlayerPassiveComponent.hpp"

namespace mmt_gd
{
class NoBouncePassive final : public IPlayerPassiveComponent
{
public:
    NoBouncePassive(GameObject& go);

    void virtual update(float deltaTime) override;

    void virtual apply(b2Contact& contact) override;

    float m_bounceFactor;
};
}

