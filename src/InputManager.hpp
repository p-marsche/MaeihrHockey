#pragma once

#include "Action.hpp"
#include "SFML/Window.hpp"

#include <unordered_map>

namespace mmt_gd
{
class InputManager
{
public:
    static InputManager& getInstance();
    InputManager(const InputManager& rhv)             = delete;
    InputManager(InputManager&& rhv)                  = delete;
    InputManager&  operator=(const InputManager& rhv) = delete;
    InputManager&& operator=(InputManager&& rhv)      = delete;

    /**
     * \brief Must be called at beginning of game update, before the update
    ´* method. The events are
     * classified and processed internally.
     * \param event a single event coming from the sf polling.
     */
    void process(const sf::Event& event);

    /**
     * \brief Must be called after all events have been processed.
     */
    void update();

    /**
     * \brief Binds an action to the given keycode and for the given player
    ´* \param action the name of the action (eg. left, jump, ...)
     * \param keyCode the keycode of the action
     * \param playerIdx the idx of the player
     */
    void bind(const std::string& action, const sf::Keyboard::Key keyCode, const int playerIdx = 0);

    void bind(const std::string& action, const int joystickAxis, const int directionValue, const int playerIdx = 0);

    void bind(const std::string& action, const int joystickButton, const int playerIdx = 0);

    void unbind(const std::string& action, const int playerIdx = 0);

    /**
     * \return Returns true if the button for the given Action is currently down.
     */
    bool isActionJustPressed(const std::string& action, int playerIdx = 0);

    /**
     * \return Returns true if the button for the given Action is currently up.
     */
    bool isActionJustReleased(const std::string& action, int playerIdx = 0);

    /**
     * \return Returns true if the button for the given Action has been pressed.
     */
    bool isActionPressed(const std::string& action, int playerIdx = 0);

    /**
     * \return Returns true if the button for the given Action has been released.
     */
    bool isActionReleased(const std::string& action, int playerIdx = 0);

    void setRenderWindow(sf::RenderWindow* window)
    {
        m_renderWindow = window;
    }

private:
    InputManager()  = default;
    ~InputManager() = default;

    Action* getActionFromName(const std::string& action, int playerIdx);
    bool    isKeyBound(sf::Keyboard::Key key);
    bool    isJoystickAxisBound(JoystickAxis axis);
    bool    isJoystickButtonBound(JoystickButton button);

    struct FrameData
    {
        std::unordered_map<Action&, bool> m_map;
    };

    FrameData m_lastFrame{};
    FrameData m_currentFrame{};
    FrameData m_eventFrame{};

    sf::RenderWindow* m_renderWindow{nullptr};

    static constexpr int                    PlayerCount = 4; ///< maximum allowed players. Can be increased if needed.
    std::unordered_map<std::string, Action> m_actionBinding[PlayerCount];
    std::unordered_map<sf::Keyboard::Key, Action&>   m_keyToAction;
    std::unordered_map<JoystickAxis, Action&>        m_joystickAxisToAction;
    std::unordered_map<JoystickButton, Action&> m_joystickButtonToAction;
};
} // namespace mmt_gd
