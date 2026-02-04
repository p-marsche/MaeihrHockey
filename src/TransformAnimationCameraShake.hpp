#pragma once

#include "EventBus.hpp"
#include "TransformAnimationComponent.hpp"

namespace mmt_gd
{
class TransformAnimationCameraShake final : public ITransformAnimation
{
public:
    TransformAnimationCameraShake(float maxAngle, float duration);
    void update(float deltaTime, sf::Transformable& transform) override;
    void activate()
    {
        m_shake = true;
    }

private:
    bool                              m_shake;
    float                             m_accumulator;
    float                             m_duration;
    float                             m_maxAngle;
};
} // namespace mmt_gd