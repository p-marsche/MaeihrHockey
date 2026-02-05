#pragma once

#include "IComponent.hpp"


namespace mmt_gd
{
class IPlayerPassiveComponent : public IComponent
{
public:
    IPlayerPassiveComponent(GameObject& go) 
        :IComponent(go) 
    {}

    bool virtual init() override
    {
        return true;
    }

    void virtual update(float deltaTime) override
    {
        return;
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

