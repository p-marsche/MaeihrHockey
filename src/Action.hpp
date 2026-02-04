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
};

struct JoystickButton
{
    int m_id;
    int m_button;

    bool operator==(const JoystickButton& other)
    {
        return (m_id == other.m_id && m_button == other.m_button);
    }
};

class Action
{
public:
    Action(const std::string& name, int playerIdx);
    const std::string& getName();
    void               addInput(const sf::Keyboard::Key key);
    void               addInput(const int joystickIndex, const int joystickAxis, const int directionValue);
    void               addInput(const int joystickIndex, const int joystickButton);
    void               removeInput(const sf::Keyboard::Key key);
    void               removeInput(const int joystickIndex, const int joystickAxis, const int directionValue);
    void               removeInput(const int joystickIndex, const int joystickButton);
    void               clearInputs();
    bool               hasInput(const sf::Keyboard::Key key) const;
    bool               hasInput(const int joystickIndex, const int joystickAxis, const int directionValue) const;
    bool               hasInput(const int joystickIndex, const int joystickButton) const;
    bool               operator==(const Action& other);

private:
    std::string                    m_name;
    int                            m_playerIdx;
    std::vector<sf::Keyboard::Key> m_keyboard;
    std::vector<JoystickAxis>      m_joystickAxis;    // index, axis, directionValue
    std::vector<JoystickButton>    m_joystickButtons; // index, button
};
} // namespace mmt_gd