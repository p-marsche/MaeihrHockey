#include "stdafx.h"

#include "Action.hpp"

namespace mmt_gd
{
Action::Action(const std::string& name, const int playerIdx) : m_name(name), m_playerIdx(playerIdx)
{
}

void Action::addInput(const sf::Keyboard::Key key)
{
    auto it = std::find(m_keyboard.begin(), m_keyboard.end(), key);
    if (it == m_keyboard.end())
        m_keyboard.emplace_back(key);
}

void Action::addInput(const JoystickAxis axis)
{
    auto it = std::find(m_joystickAxis.begin(), m_joystickAxis.end(), axis);
    if (it == m_joystickAxis.end())
        m_joystickAxis.emplace_back(axis);
}

void Action::addInput(const JoystickButton button)
{
    auto it = std::find(m_joystickButtons.begin(), m_joystickButtons.end(), button);
    if (it == m_joystickButtons.end())
        m_joystickButtons.emplace_back(button);
}

void Action::removeInput(const sf::Keyboard::Key key)
{
    auto it = std::find(m_keyboard.begin(), m_keyboard.end(), key);
    if (it != m_keyboard.end())
        m_keyboard.erase(it);
}

void Action::removeInput(const JoystickAxis axis)
{
    auto it = std::find(m_joystickAxis.begin(), m_joystickAxis.end(), axis);
    if (it != m_joystickAxis.end())
        m_joystickAxis.erase(it);
}

void Action::removeInput(const JoystickButton button)
{
    auto it = std::find(m_joystickButtons.begin(), m_joystickButtons.end(), button);
    if (it != m_joystickButtons.end())
        m_joystickButtons.erase(it);
}

void Action::clearInputs()
{
    m_keyboard.clear();
    m_joystickAxis.clear();
    m_joystickButtons.clear();
}

bool Action::hasInput(const sf::Keyboard::Key key) const
{
    auto it = std::find(m_keyboard.begin(), m_keyboard.end(), key);
    return it != m_keyboard.end();
}

bool Action::hasInput(JoystickAxis axis) const
{
    auto it = std::find(m_joystickAxis.begin(), m_joystickAxis.end(), axis);
    return it != m_joystickAxis.end();
}

bool Action::hasInput(JoystickButton button) const
{
    auto it = std::find(m_joystickButtons.begin(), m_joystickButtons.end(), button);
    return it != m_joystickButtons.end();
}

bool Action::operator==(const Action& other)
{
    return m_name == other.m_name && m_playerIdx == other.m_playerIdx;
}

bool Action::operator==(const Action& other) const
{
    return m_name == other.m_name && m_playerIdx == other.m_playerIdx;
}
} // namespace mmt_gd