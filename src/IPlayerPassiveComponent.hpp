#pragma once

#include "IComponent.hpp"


namespace mmt_gd
{
class IPlayerPassiveComponent : public IComponent
{
public:

    IPlayerPassiveComponent(GameObject& go) 
        :IComponent(go) 
        , m_enabled(false)
    {}

    bool virtual init() override
    {
        return true;
    }

    void virtual update(float deltaTime) override
    {
        return;
    }

    void virtual apply(b2Contact& contact)
    {
        return;
    }

    void SetEnable(bool enabled)
    {
        m_enabled = enabled;
    }

protected:
    bool m_enabled;
};
}
