//This code was made for the Multimedia Project 2a for the Multimedia Technology class at Fachhochschule Salzburg, by Pascal Veselka & Pascal Marsche
#pragma once

#include "Action.hpp"
#include <memory>

namespace mmt_gd
{
class ActionHash
{
public:
    size_t operator()(const Action& a) const;
};

class ActionRefHash
{
public:
    size_t operator()(const std::weak_ptr<Action>& a) const;
};

class AxisHash
{
public:
    size_t operator()(const JoystickAxis& a) const;
};

class ButtonHash
{
public:
    size_t operator()(const JoystickButton& a) const;
};
} // namespace mmt_gd