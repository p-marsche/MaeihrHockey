#include "stdafx.h"

#include "Action.hpp"

namespace mmt_gd
{
Action::Action(std::string name) : m_name(name)
{
}

std::string Action::getName()
{
    return m_name;
}

void Action::addInput(sf::Keyboard::Key key)
{
    auto it = std::find(m_keyboard.begin(), m_keyboard.end(), key);
    if (it == m_keyboard.end())
        m_keyboard.emplace_back(key);
}

void Action::addInput(int joystickAxis, int directionValue)
{
    std::pair<int, int> input{joystickAxis, directionValue};
    auto                it = std::find(m_joystickAxis.begin(), m_joystickAxis.end(), input);
    if (it == m_joystickAxis.end())
        m_joystickAxis.emplace_back(input);
}

void Action::addInput(int joystickButton)
{
    auto it = std::find(m_joystickButtons.begin(), m_joystickButtons.end(), joystickButton);
    if (it == m_joystickButtons.end())
        m_joystickButtons.emplace_back(joystickButton);
}

void Action::removeInput(sf::Keyboard::Key key)
{
    auto it = std::find(m_keyboard.begin(), m_keyboard.end(), key);
    if (it != m_keyboard.end())
        m_keyboard.erase(it);
}

void Action::removeInput(int joystickAxis, int directionValue)
{
    auto it = std::find(m_joystickAxis.begin(), m_joystickAxis.end(), std::pair<int, int>(joystickAxis, directionValue));
    if (it != m_joystickAxis.end())
        m_joystickAxis.erase(it);
}

void Action::removeInput(int joystickButton)
{
    auto it = std::find(m_joystickButtons.begin(), m_joystickButtons.end(), joystickButton);
    if (it != m_joystickButtons.end())
        m_joystickButtons.erase(it);
}

void Action::clearInputs()
{
    m_keyboard.clear();
    m_joystickAxis.clear();
    m_joystickButtons.clear();
}

bool Action::hasInput(sf::Keyboard::Key key)
{
    auto it = std::find(m_keyboard.begin(), m_keyboard.end(), key);
    return it != m_keyboard.end();
}

bool Action::hasInput(int joystickAxis, int directionValue)
{
    auto it = std::find(m_joystickAxis.begin(), m_joystickAxis.end(), std::pair<int, int>(joystickAxis, directionValue));
    return it != m_joystickAxis.end();
}

bool Action::hasInput(int joystickButton)
{
    auto it = std::find(m_joystickButtons.begin(), m_joystickButtons.end(), joystickButton);
    return it != m_joystickButtons.end();
}

} // namespace mmt_gd