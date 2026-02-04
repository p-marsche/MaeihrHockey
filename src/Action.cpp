#include "stdafx.h"

#include "Action.hpp"

namespace mmt_gd
{
Action::Action(const std::string& name, const int playerIdx) : m_name(name), m_playerIdx(playerIdx)
{
}

const std::string& Action::getName()
{
    return m_name;
}

void Action::addInput(const sf::Keyboard::Key key)
{
    auto it = std::find(m_keyboard.begin(), m_keyboard.end(), key);
    if (it == m_keyboard.end())
        m_keyboard.emplace_back(key);
}

void Action::addInput(const int joystickIndex, const int joystickAxis, const int directionValue)
{
    JoystickAxis input = {joystickIndex, joystickAxis, directionValue};
    auto         it    = std::find(m_joystickAxis.begin(), m_joystickAxis.end(), input);
    if (it == m_joystickAxis.end())
        m_joystickAxis.emplace_back(input);
}

void Action::addInput(const int joystickIndex, const int joystickButton)
{
    JoystickButton input{joystickIndex, joystickButton};
    auto                it = std::find(m_joystickButtons.begin(), m_joystickButtons.end(), input);
    if (it == m_joystickButtons.end())
        m_joystickButtons.emplace_back(input);
}

void Action::removeInput(const sf::Keyboard::Key key)
{
    auto it = std::find(m_keyboard.begin(), m_keyboard.end(), key);
    if (it != m_keyboard.end())
        m_keyboard.erase(it);
}

void Action::removeInput(const int joystickIndex, const int joystickAxis, const int directionValue)
{
    auto it = std::find(m_joystickAxis.begin(),
                        m_joystickAxis.end(),
                        JoystickAxis{joystickIndex, joystickAxis, directionValue});
    if (it != m_joystickAxis.end())
        m_joystickAxis.erase(it);
}

void Action::removeInput(const int joystickIndex, const int joystickButton)
{
    auto it = std::find(m_joystickButtons.begin(), m_joystickButtons.end(), JoystickButton{joystickIndex, joystickButton});
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

bool Action::hasInput(const int joystickIndex, const int joystickAxis, const int directionValue) const
{
    auto it = std::find(m_joystickAxis.begin(),
                        m_joystickAxis.end(),
                        JoystickAxis{joystickIndex, joystickAxis, directionValue});
    return it != m_joystickAxis.end();
}

bool Action::hasInput(const int joystickIndex, const int joystickButton) const
{
    auto it = std::find(m_joystickButtons.begin(), m_joystickButtons.end(), JoystickButton{joystickIndex, joystickButton});
    return it != m_joystickButtons.end();
}

bool Action::operator==(const Action& other)
{
    return m_name == other.m_name && m_playerIdx == other.m_playerIdx;
}
} // namespace mmt_gd