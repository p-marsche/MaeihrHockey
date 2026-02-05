#include "stdafx.h"

#include "Game.hpp"

#include "DebugDraw.hpp"
#include "InputManager.hpp"
#include "MainState.hpp"
#include "MenuState.hpp"

#include <sstream>


namespace mmt_gd
{
using namespace std;

void Game::run()
{
    Instrumentor::instance().beginSession("startup", "startup.json");

    if (!init())
    {
        return;
    }
    Instrumentor::instance().endSession();
    Instrumentor::instance().beginSession("runtime", "runtime.json");

    while (m_windowHandler.m_window.isOpen())
    {
        PROFILE_SCOPE("Frame");

        {
            PROFILE_SCOPE("Process Events");

            // process events in the input manager
            sf::Event event{};
            while (m_windowHandler.m_window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    shutdown();
                    m_windowHandler.m_window.close();
                    return;
                }
                else if (event.type == sf::Event::Resized)
                    m_windowHandler.resizeWindow();

                m_inputManager->process(event);
                m_gui.handleEvent(event);
            }
        }
        update();
        draw();
    }

    shutdown();
}

void Game::initInputManager()
{
    m_inputManager = &InputManager::getInstance();

    m_inputManager->bind("Exit", sf::Keyboard::Escape);
    m_inputManager->bind("Select", sf::Keyboard::Space);

    // May move to view later on
    m_inputManager->bind("up", sf::Keyboard::W, 0);
    m_inputManager->bind("left", sf::Keyboard::A, 0);
    m_inputManager->bind("down", sf::Keyboard::S, 0);
    m_inputManager->bind("right", sf::Keyboard::D, 0);
    m_inputManager->bind("switch", sf::Keyboard::R, 0);
    m_inputManager->bind("ability", sf::Keyboard::Q, 0);

    m_inputManager->bind("up", JoystickMap::Direction::Up, 0);
    m_inputManager->bind("left", JoystickMap::Direction::Left, 0);
    m_inputManager->bind("down", JoystickMap::Direction::Down, 0);
    m_inputManager->bind("right", JoystickMap::Direction::Right, 0);
    m_inputManager->bind("switch", 0, 0);
    m_inputManager->bind("ability", 1, 0);

    m_inputManager->bind("up", sf::Keyboard::Up, 1);
    m_inputManager->bind("left", sf::Keyboard::Left, 1);
    m_inputManager->bind("down", sf::Keyboard::Down, 1);
    m_inputManager->bind("right", sf::Keyboard::Right, 1);
    m_inputManager->bind("switch", sf::Keyboard::RShift, 1);
    m_inputManager->bind("ability", sf::Keyboard::Num0, 1);

    m_inputManager->bind("up", JoystickMap::Direction::Up, 1);
    m_inputManager->bind("left", JoystickMap::Direction::Left, 1);
    m_inputManager->bind("down", JoystickMap::Direction::Down, 1);
    m_inputManager->bind("right", JoystickMap::Direction::Right, 1);
    m_inputManager->bind("switch", 0, 1);
    m_inputManager->bind("ability", 1, 1);

    m_inputManager->bind("debugdraw", sf::Keyboard::F1, 0);
}

bool Game::init()
{
    PROFILE_FUNCTION();

    initInputManager();

    m_debugDraw = &DebugDraw::getInstance();

    //
    tgui::Gui* gui = &m_gui;
    m_gameStateManager.registerState("MenuState", make_shared<MenuState>(&m_gameStateManager, this, gui));
    m_gameStateManager.registerState("MainState", make_shared<MainState>(&m_gameStateManager, this, gui, 2));

    //
    m_windowHandler.init(m_config.m_windowName, m_config.m_resolution.x, m_config.m_resolution.y, gui);
    m_gui.setTarget(m_windowHandler.m_window);

    m_inputManager->setRenderWindow(&m_windowHandler.m_window);
    m_inputManager->bind("fullscreen", sf::Keyboard::F11);

    m_gameStateManager.setState("MenuState");

    return true;
}

void Game::update()
{
    PROFILE_FUNCTION();

    // starts the clock
    static sf::Clock clock;
    // restart takes time
    const auto deltaTime        = clock.restart();
    const auto deltaTimeSeconds = deltaTime.asSeconds();

    // must be first call
    m_inputManager->update();

    if (m_inputManager->isActionJustPressed("fullscreen"))
        m_windowHandler.toggleFullscreen();

    m_gameStateManager.update(deltaTimeSeconds);

    m_debugDraw->update(deltaTimeSeconds);

    std::ostringstream ss;
    m_fps.update();
    ss << m_config.m_windowName << " | FPS: " << m_fps.getFps();

    m_windowHandler.m_window.setTitle(ss.str());
}

void Game::draw()
{
    PROFILE_FUNCTION();

    m_windowHandler.m_window.clear(sf::Color::Black);

    m_gameStateManager.draw();

    m_debugDraw->draw(m_windowHandler.m_window);

    m_gui.draw();

    m_windowHandler.m_window.display();
}

void Game::shutdown() const
{
    m_gameStateManager.shutdown();
}
} // namespace mmt_gd
