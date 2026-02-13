//This code was made for the Multimedia Project 2a for the Multimedia Technology class at Fachhochschule Salzburg, by Pascal Veselka & Pascal Marsche
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

