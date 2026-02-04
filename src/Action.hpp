#pragma once

#include "JoystickMap.hpp"

namespace mmt_gd
{
struct JoystickAxis
{
    int m_id;
    int m_axis;
    int m_direction;

    bool operator==(const JoystickAxis& other)
    {
        return (m_id == other.m_id && m_axis == other.m_axis && m_direction == other.m_direction);
    }

    bool operator==(const JoystickAxis& other) const
    {
        return (m_id == other.m_id && m_axis == other.m_axis && m_direction == other.m_direction);
    }
};

struct JoystickButton
{
    int m_id;
    int m_button;

    bool operator==(const JoystickButton& other)
    {
        return (m_id == other.m_id && m_button == other.m_button);
    }

    bool operator==(const JoystickButton& other) const
    {
        return (m_id == other.m_id && m_button == other.m_button);
    }
};

class Action
{
public:
    Action(const std::string& name, int playerIdx);
    std::string        m_name;
    int                m_playerIdx;
    void               addInput(const sf::Keyboard::Key key);
    void               addInput(const JoystickAxis axis);
    void               addInput(const JoystickButton button);
    void               removeInput(const sf::Keyboard::Key key);
    void               removeInput(const JoystickAxis axis);
    void               removeInput(const JoystickButton button);
    void               clearInputs();
    bool               hasInput(const sf::Keyboard::Key key) const;
    bool               hasInput(const JoystickAxis axis) const;
    bool               hasInput(const JoystickButton button) const;
    bool               operator==(const Action& other);
    bool               operator==(const Action& other) const;

private:
    std::vector<sf::Keyboard::Key> m_keyboard;
    std::vector<JoystickAxis>      m_joystickAxis;    // index, axis, directionValue
    std::vector<JoystickButton>    m_joystickButtons; // index, button
};
} // namespace mmt_gd