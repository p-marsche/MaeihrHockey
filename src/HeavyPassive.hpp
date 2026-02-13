//This code was made for the Multimedia Project 2a for the Multimedia Technology class at Fachhochschule Salzburg, by Pascal Veselka & Pascal Marsche
#pragma once

#include "IPlayerPassiveComponent.hpp"

namespace mmt_gd
{
class HeavyPassive final : public IPlayerPassiveComponent
{
public:
    HeavyPassive(GameObject& go);

    void virtual update(float deltaTime) override;
};
};// namespace mmt_gd

