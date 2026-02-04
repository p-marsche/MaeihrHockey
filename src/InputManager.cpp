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
            if (isKeyBound(event.key.code))
                m_eventFrame.m_map[*m_keyToAction[event.key.code].lock()] = true;
            break;
        case sf::Event::KeyReleased:
            if (isKeyBound(event.key.code))
                m_eventFrame.m_map[*m_keyToAction[event.key.code].lock()] = false;
            break;
        case sf::Event::JoystickMoved:
            if (isJoystickAxisBound(JoystickAxis{(int)event.joystickMove.joystickId,
                                                 (int)event.joystickMove.axis,
                                                 (int)event.joystickMove.position}))
            {
                m_eventFrame.m_map[*m_joystickAxisToAction[JoystickAxis{(int)event.joystickMove.joystickId,
                                                                        (int)event.joystickMove.axis,
                                                                        (int)event.joystickMove.position}]
                                        .lock()] = true;
            }
            else if (event.joystickMove.position < JoystickMap::ZERO_POS &&
                     event.joystickMove.position > JoystickMap::ZERO_NEG)
            {
                resetAxis(event);
            }
            break;
        case sf::Event::JoystickButtonPressed:
            if (isJoystickButtonBound(JoystickButton{(int)event.joystickButton.joystickId, (int)event.joystickButton.button}))
                m_eventFrame.m_map[*m_joystickButtonToAction[JoystickButton{(int)event.joystickButton.joystickId,
                                                                            (int)event.joystickButton.button}]
                                        .lock()] = true;
            break;
        case sf::Event::JoystickButtonReleased:
            if (isJoystickButtonBound(JoystickButton{(int)event.joystickButton.joystickId, (int)event.joystickButton.button}))
                m_eventFrame.m_map[*m_joystickButtonToAction[JoystickButton{(int)event.joystickButton.joystickId,
                                                                            (int)event.joystickButton.button}]
                                        .lock()] = false;
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
            .emplace(action, new Action(action, playerIdx));
    m_actionBinding[playerIdx][action]->addInput(keyCode);

    if (isKeyBound(keyCode))
        std::clog << "WARNING: Double control binding. Removed old binding.";
    m_keyToAction[keyCode] = m_actionBinding[playerIdx][action];
}

void InputManager::bind(const std::string& action, const int joystickButton, const int playerIdx)
{
    ffAssertMsg(playerIdx < PlayerCount, "player out of bounds")

        auto btn = JoystickButton{playerIdx, joystickButton};
    if (m_actionBinding[playerIdx].find(action) == m_actionBinding[playerIdx].end())
        m_actionBinding[playerIdx].emplace(action, new Action(action, playerIdx));
    m_actionBinding[playerIdx][action]->addInput(btn);

    if (isJoystickButtonBound(btn))
        std::clog << "WARNING: Double control binding. Removed old binding.";
    m_joystickButtonToAction[btn] = m_actionBinding[playerIdx][action];
}

void InputManager::bind(const std::string& action, const int joystickAxis, const int directionValue, const int playerIdx)
{
    ffAssertMsg(playerIdx < PlayerCount, "player out of bounds")

        auto axis = JoystickAxis{playerIdx, joystickAxis, directionValue};
    if (m_actionBinding[playerIdx].find(action) == m_actionBinding[playerIdx].end())
        m_actionBinding[playerIdx].emplace(action, new Action(action, playerIdx));
    m_actionBinding[playerIdx][action]->addInput(axis);

    if (isJoystickAxisBound(axis))
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
        return it->second.get();
    }
    return nullptr;
}

bool InputManager::isKeyBound(sf::Keyboard::Key key)
{
    return m_keyToAction.find(key) != m_keyToAction.end();
}

bool InputManager::isJoystickAxisBound(JoystickAxis axis)
{
    return m_joystickAxisToAction.find(axis) != m_joystickAxisToAction.end();
}

bool InputManager::isJoystickButtonBound(JoystickButton button)
{
    return m_joystickButtonToAction.find(button) != m_joystickButtonToAction.end();
}

void InputManager::resetAxis(const sf::Event& event)
{
    if (isJoystickAxisBound(
            JoystickAxis{(int)event.joystickMove.joystickId, (int)event.joystickMove.axis, JoystickMap::THRESHOLD_POS}))
        m_eventFrame
            .m_map[*m_joystickAxisToAction[JoystickAxis{(int)event.joystickMove.joystickId, (int)event.joystickMove.axis, JoystickMap::THRESHOLD_POS}]
                        .lock()] = false;
    else if (isJoystickAxisBound(
                 JoystickAxis{(int)event.joystickMove.joystickId, (int)event.joystickMove.axis, JoystickMap::THRESHOLD_NEG}))
        m_eventFrame
            .m_map[*m_joystickAxisToAction[JoystickAxis{(int)event.joystickMove.joystickId, (int)event.joystickMove.axis, JoystickMap::THRESHOLD_NEG}]
                        .lock()] = false;
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
    return !m_currentFrame.m_map[*inputAction] && m_lastFrame.m_map[*inputAction];
}

bool InputManager::isActionPressed(const std::string& action, const int playerIdx)
{
    auto inputAction = getActionFromName(action, playerIdx);
    if (inputAction == nullptr)
        return false;
    return m_currentFrame.m_map[*inputAction];
}

bool InputManager::isActionReleased(const std::string& action, const int playerIdx)
{
    auto inputAction = getActionFromName(action, playerIdx);
    if (inputAction == nullptr)
        return false;
    return !m_currentFrame.m_map[*inputAction];
}
} // namespace mmt_gd