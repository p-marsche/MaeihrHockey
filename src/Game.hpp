#pragma once

#include "FPS.hpp"
#include "GameStateManager.hpp"
#include "WindowHandler.hpp"

#include <string>

namespace mmt_gd
{
class InputManager;
class DebugDraw;

class Game
{
public:
    struct Config
    {
        sf::Vector2i m_resolution{1920, 1080};
        std::string  m_windowName = "AirHockey";
    };

    Config& getConfig()
    {
        return m_config;
    }

    sf::RenderWindow& getWindow()
    {
        return m_windowHandler.m_window;
    }

    tgui::Gui& getGui()
    {
        return m_gui;
    }

    void run();

private:
    bool init();
    void initInputManager();
    void update();
    void draw();
    void shutdown() const;

    Config m_config;

    WindowHandler m_windowHandler;
    GameStateManager m_gameStateManager;

    InputManager* m_inputManager = nullptr;
    DebugDraw*    m_debugDraw    = nullptr;
    Fps           m_fps;
    tgui::Gui     m_gui;
};
} // namespace mmt_gd
