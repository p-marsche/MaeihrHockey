//This code was made for the Multimedia Project 2a for the Multimedia Technology class at Fachhochschule Salzburg, by Pascal Veselka & Pascal Marsche
#pragma once

#include "IPlayerPassiveComponent.hpp"

namespace mmt_gd
{
class HighBouncePassive final : public IPlayerPassiveComponent
{
public:
    HighBouncePassive(GameObject& go);

    void virtual apply(b2Contact& contact) override;

    float m_bounceFactor;
};
}

