#include "stdafx.h"

#include "MainState.hpp"

#include "AssetManager.hpp"
#include "CameraRenderComponent.hpp"
#include "ColliderComponent.hpp"
#include "Game.hpp"
#include "InputManager.hpp"
#include "TileMapLoader.hpp"
#include "Tileson.hpp"
#include "TransformAnimationComponent.hpp"
#include "TransformAnimationConstantMotion.hpp"

#include <memory>
#include <thread>

namespace mmt_gd
{
MainState::MainState(GameStateManager* gameStateManager, Game* game, tgui::Gui* gui) :
GameState(gameStateManager, game, gui),
m_spriteManager(game->getWindow())
{
    initGui();
}

void MainState::initGui()
{
    m_guiGroup = tgui::Group::create();
    std::string filename = "matchUI.txt";
    m_guiGroup->loadWidgetsFromFile(Config::guiPath + filename);
    m_gui->add(m_guiGroup);
}

void MainState::init()
{
    PROFILE_FUNCTION();

    m_guiGroup->setVisible(true);
    m_timerSeconds = 300;
    m_accumulator  = 0.f;

    m_gameObjectManager.init();
    m_spriteManager.init();
    m_physicsManager.init();

    // Load tile map
    tson::Tileson tileson;
    const auto    map = tileson.parse(fs::path("../assets/arena-1.tmj"));
    if (map->getStatus() == tson::ParseStatus::OK)
    {
        TileMapLoader::loadTileLayers(map, m_spriteManager);
        TileMapLoader::loadObjectLayers(map, m_spriteManager);
    }
    else
    {
        sf::err() << "Could not load tile map\n";
    }

    // create camera
    const auto camera          = GameObject::create("Camera");
    const auto renderComponent = camera->addComponent<CameraRenderComponent>(*camera,
                                                                             m_game->getWindow(),
                                                                             m_game->getWindow().getView());
    camera->setPosition(sf::Vector2f(m_game->getWindow().getSize().x / 2, m_game->getWindow().getSize().y / 2));

    if (!camera->init())
        FF_ERROR_MSG("Could not initialize camera");

    m_gameObjectManager.addGameObject(camera);
    m_spriteManager.setCamera(renderComponent.get());
}

void MainState::update(const float deltaTime)
{
    PROFILE_FUNCTION();
    if (InputManager::getInstance().isKeyPressed("Exit"))
    {
        m_gameStateManager->setState("MenuState");
        return;
    }

    EventBus::getInstance().processEvents(deltaTime);
    m_gameObjectManager.update(deltaTime);
    m_physicsManager.update(deltaTime);

    updateTimer(deltaTime);
}

void MainState::updateTimer(const float deltaTime)
{
    m_accumulator += deltaTime;
    while (m_accumulator > 1.0f)
    {
        m_timerSeconds--;

        int seconds = m_timerSeconds % 60;
        int minutes = m_timerSeconds / 60;
        auto time    = tgui::String(minutes) + " : " + tgui::String(seconds);
        m_guiGroup->get<tgui::Label>("Timer")->setText(time);

        m_accumulator--;
    }

    if (m_timerSeconds < 1)
        std::cout << "Game ends" << std::endl;
}

void MainState::draw()
{
    PROFILE_FUNCTION();
    m_spriteManager.draw();
}

void MainState::exit()
{
    PROFILE_FUNCTION();
    m_guiGroup->setVisible(false);
    m_physicsManager.shutdown();
    m_spriteManager.shutdown();
    m_gameObjectManager.shutdown();
}
} // namespace mmt_gd
