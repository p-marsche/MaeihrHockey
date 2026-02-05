#pragma once

#include "EventBus.hpp"
#include "TransformAnimationComponent.hpp"

namespace mmt_gd
{
class TransformAnimationCameraShake final : public ITransformAnimation
{
public:
    TransformAnimationCameraShake(sf::Vector2f magnitude, float duration);
    void update(float deltaTime, sf::Transformable& transform) override;
    void activate()
    {
        m_shake = true;
    }

private:
    bool         m_shake;
    float        m_accumulator;
    float        m_duration;
    int          m_count;
    sf::Vector2f m_magnitude[4];
    sf::Vector2f m_originalPosition;
};
} // namespace mmt_gd