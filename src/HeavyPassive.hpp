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

