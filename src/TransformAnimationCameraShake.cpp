#include "stdafx.h"

#include "TransformAnimationCameraShake.hpp"

#include "GoalEvent.hpp"

namespace mmt_gd
{
TransformAnimationCameraShake::TransformAnimationCameraShake(sf::Vector2f magnitude, float duration) :
m_duration(duration),
m_accumulator(0),
m_shake(false),
m_count(0)
{
    magnitude.x      = std::abs(magnitude.x);
    magnitude.y      = std::abs(magnitude.y);
    m_magnitude[0]   = magnitude;
    m_magnitude[1].x = -2 * magnitude.x;
    m_magnitude[1].y = 0;
    m_magnitude[2].x = 0;
    m_magnitude[2].y = 2 * magnitude.y;
    m_magnitude[3].x = 2 * magnitude.x;
    m_magnitude[3].y = 0;
}

void TransformAnimationCameraShake::update(float deltaTime, sf::Transformable& transform)
{
    if (m_shake)
    {
        if (m_accumulator == 0) ///< right after activation, no movement happened yet
            m_originalPosition = transform.getPosition();
        if (m_accumulator >= m_duration)
        {
            m_accumulator = 0;
            m_shake       = false;
            m_count       = 0;
            transform.setPosition(m_originalPosition);
        }

        if (m_count == 4)
        {
            transform.move((m_originalPosition + m_magnitude[0]) - transform.getPosition());
            m_count = 0;
        }
        else
            transform.move(m_magnitude[m_count]);

        m_accumulator += deltaTime;
        m_count++;
    }
}
} // namespace mmt_gd
