#pragma once

#include "JoystickMap.hpp"

namespace mmt_gd
{
class Action
{
public:
    Action(std::string name);
    std::string getName();
    void        addInput(sf::Keyboard::Key key);
    void        addInput(int joystickAxis, int directionValue);
    void        addInput(int joystickButton);
    void        removeInput(sf::Keyboard::Key key);
    void        removeInput(int joystickAxis, int directionValue);
    void        removeInput(int joystickButton);
    void        clearInputs();
    bool        hasInput(sf::Keyboard::Key key);
    bool        hasInput(int joystickAxis, int directionValue);
    bool        hasInput(int joystickButton);

private:
    std::string m_name;
    std::vector<sf::Keyboard::Key>   m_keyboard;
    std::vector<std::pair<int, int>> m_joystickAxis;
    std::vector<int>                 m_joystickButtons;
};
} // namespace mmt_gd