#include "stdafx.h"

#include "GoalEvent.hpp"
#include "TransformAnimationCameraShake.hpp"

namespace mmt_gd
{
TransformAnimationCameraShake::TransformAnimationCameraShake(float maxAngle, float duration) :
m_maxAngle(maxAngle),
m_duration(duration),
m_accumulator(0),
m_shake(false)
{
}

void TransformAnimationCameraShake::update(float deltaTime, sf::Transformable& transform)
{
    if (m_shake)
    {
        if (m_accumulator >= m_duration)
        {
            m_accumulator = 0;
            m_shake       = false;
            transform.setRotation(0);
        }
        transform.rotate(m_maxAngle);
        //m_maxAngle = -m_maxAngle;
        m_accumulator += deltaTime;
    }
}
} // namespace mmt_gd
