#pragma once

#include "IComponent.hpp"
#include "PlayerConfig.hpp"


namespace mmt_gd
{
class IPlayerPassiveComponent : public IComponent
{
public:

    IPlayerPassiveComponent(GameObject& go, PaddlePassive passive) 
        :IComponent(go) 
        , m_enabled(false)
        , m_passive(passive)
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

    PaddlePassive getType()
    {
        return m_passive;
    }

protected:
    bool m_enabled;
    PaddlePassive m_passive;
};
}
