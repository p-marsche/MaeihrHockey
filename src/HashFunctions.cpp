#include "stdafx.h"

#include "HashFunctions.hpp"

namespace mmt_gd
{
size_t ActionHash::operator()(const Action& a) const
{
    return (std::hash<std::string>()(a.m_name)) ^ (std::hash<int>()(a.m_playerIdx));
}
size_t AxisHash::operator()(const JoystickAxis& ja) const
{
    return (std::hash<int>()(ja.m_id)) ^ (std::hash<int>()((int)ja.m_direction));
}
size_t ButtonHash::operator()(const JoystickButton& jb) const
{
    return (std::hash<int>()(jb.m_id)) ^ (std::hash<int>()(jb.m_button));
}
size_t ActionRefHash::operator()(const std::weak_ptr<Action>& a) const
{
    return (std::hash<std::string>()(a.lock()->m_name)) ^ (std::hash<int>()(a.lock()->m_playerIdx));
}
} // namespace mmt_gd