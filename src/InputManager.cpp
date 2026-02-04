#include "stdafx.h"

#include "InputManager.hpp"

namespace mmt_gd
{
InputManager& InputManager::getInstance()
{
    static InputManager instance;
    return instance;
}

void InputManager::process(const sf::Event& event)
{
    switch (event.type) // NOLINT(clang-diagnostic-switch-enum)
    {
        case sf::Event::KeyPressed:

            break;
        case sf::Event::KeyReleased:

            break;
        case sf::Event::JoystickMoved:

            break;
        case sf::Event::JoystickButtonPressed:

            break;
        case sf::Event::JoystickButtonReleased:

            break;
        case sf::Event::JoystickConnected:
            std::cout << "Joystick connected " << event.joystickConnect.joystickId << " ("
                      << sf::Joystick::getButtonCount(0) << " buttons)" << std::endl;
            break;
        case sf::Event::JoystickDisconnected:
            std::cout << "Joystick disconnected " << event.joystickConnect.joystickId << std::endl;
            break;
        default:
            break;
    }
}

void InputManager::update()
{
    m_lastFrame    = m_currentFrame;
    m_currentFrame = m_eventFrame;
}

void InputManager::bind(const std::string& action, const sf::Keyboard::Key keyCode, const int playerIdx)
{
    ffAssertMsg(playerIdx < PlayerCount, "player out of bounds")

        if (m_actionBinding[playerIdx].find(action) == m_actionBinding[playerIdx].end()) m_actionBinding[playerIdx]
            .emplace(action, Action(action, playerIdx));
    m_actionBinding[playerIdx][action].addInput(keyCode);

    if (m_keyToAction.find(keyCode) != m_keyToAction.end())
        std::clog << "WARNING: Double control binding. Removed old binding.";
    m_keyToAction[keyCode] = m_actionBinding[playerIdx][action];
}

void InputManager::bind(const std::string& action, const int joystickButton, const int playerIdx)
{
    ffAssertMsg(playerIdx < PlayerCount, "player out of bounds")

        if (m_actionBinding[playerIdx].find(action) == m_actionBinding[playerIdx].end()) m_actionBinding[playerIdx]
            .emplace(action, Action(action, playerIdx));
    m_actionBinding[playerIdx][action].addInput(playerIdx, joystickButton);

    auto btn = JoystickButton{playerIdx, joystickButton};
    if (m_joystickButtonToAction.find(btn) != m_joystickButtonToAction.end())
        std::clog << "WARNING: Double control binding. Removed old binding.";
    m_joystickButtonToAction[btn] = m_actionBinding[playerIdx][action];
}

void InputManager::bind(const std::string& action, const int joystickAxis, const int directionValue, const int playerIdx)
{
    ffAssertMsg(playerIdx < PlayerCount, "player out of bounds")

        if (m_actionBinding[playerIdx].find(action) == m_actionBinding[playerIdx].end()) m_actionBinding[playerIdx]
            .emplace(action, Action(action, playerIdx));
    m_actionBinding[playerIdx][action].addInput(playerIdx, joystickAxis, directionValue);

    auto axis = JoystickAxis{playerIdx, joystickAxis, directionValue};
    if (m_joystickAxisToAction.find(axis) != m_joystickAxisToAction.end())
        std::clog << "WARNING: Double control binding. Removed old binding.";
    m_joystickAxisToAction[axis] = m_actionBinding[playerIdx][action];
}

void InputManager::unbind(const std::string& action, const int playerIdx)
{
    ffAssertMsg(playerIdx < PlayerCount, "player out of bounds") m_actionBinding[playerIdx].erase(action);
}

Action* InputManager::getActionFromName(const std::string& action, const int playerIdx)
{
    ffAssertMsg(playerIdx < PlayerCount, "player out of bounds")

        const auto it = m_actionBinding[playerIdx].find(action);
    if (it != m_actionBinding[playerIdx].end())
    {
        return &(it->second);
    }
    return nullptr;
}

bool InputManager::isActionJustPressed(const std::string& action, const int playerIdx)
{
    auto inputAction = getActionFromName(action, playerIdx);
    if (inputAction == nullptr)
        return false;

    return m_currentFrame.m_map[*inputAction] && m_lastFrame.m_map[*inputAction];
}

bool InputManager::isActionJustReleased(const std::string& action, const int playerIdx)
{
    auto inputAction = getActionFromName(action, playerIdx);
    if (inputAction == nullptr)
        return false;
    

}

bool InputManager::isActionPressed(const std::string& action, const int playerIdx)
{
    auto inputAction = getActionFromName(action, playerIdx);
    if (inputAction == nullptr)
        return false;
    return isKeyPressed();
}

bool InputManager::isActionReleased(const std::string& action, const int playerIdx)
{
    auto inputAction = getActionFromName(action, playerIdx);
    if (inputAction == nullptr)
        return false;
    return isKeyReleased();
}
} // namespace mmt_gd