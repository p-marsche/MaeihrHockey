//This code was made for the Multimedia Project 2a for the Multimedia Technology class at Fachhochschule Salzburg, by Pascal Veselka & Pascal Marsche
#pragma once

#include "IComponent.hpp"

namespace mmt_gd
{
class IAnimationComponent : public IComponent
{
public:
    using Ptr = std::shared_ptr<IAnimationComponent>;

    using IComponent::IComponent;
};
} // namespace mmt_gd
