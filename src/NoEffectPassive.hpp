#pragma once

#include "IPlayerPassiveComponent.hpp"

namespace mmt_gd
{
class NoEffectPassive final : public IPlayerPassiveComponent
{
public:
    NoEffectPassive(GameObject& go) :
        IPlayerPassiveComponent(go, PaddlePassive::NOTHING)
    {
    }

    void virtual apply()
    {
        return;
    }
    void virtual revert()
    {
        return;
    }
};
}

