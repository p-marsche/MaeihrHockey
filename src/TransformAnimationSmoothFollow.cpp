//This code was made for the Multimedia Project 2a for the Multimedia Technology class at Fachhochschule Salzburg, by Pascal Veselka & Pascal Marsche
#include "stdafx.h"

#include "TransformAnimationSmoothFollow.hpp"

#include "GameObject.hpp"

#include <utility>


mmt::TransformAnimationSmoothFollow::TransformAnimationSmoothFollow(mmt_gd::GameObject::Ptr target, const float smoothFactor) :
m_target(std::move(target)),
m_smoothFactor(smoothFactor)
{
}

void mmt::TransformAnimationSmoothFollow::update(float deltaTime, sf::Transformable& transform)
{
    if (m_target && m_target->isMarkedForDelete())
    {
        m_target = nullptr;
    }

    if (!m_target)
    {
        return;
    }

    const auto toTarget = m_target->getPosition() - transform.getPosition();

    transform.move(toTarget * m_smoothFactor * deltaTime);
}
