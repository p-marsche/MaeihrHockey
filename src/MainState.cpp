#include "stdafx.h"

#include "MainState.hpp"

#include "AssetManager.hpp"
#include "CameraRenderComponent.hpp"
#include "ColliderComponent.hpp"
#include "EventBus.hpp"
#include "Game.hpp"
#include "GoalEvent.hpp"
#include "InputManager.hpp"
#include "ResizeEvent.hpp"
#include "TileMapLoader.hpp"
#include "Tileson.hpp"
#include "TransformAnimationCameraShake.hpp"
#include "TransformAnimationComponent.hpp"

#include <memory>

namespace mmt_gd
{
int constexpr ROUND_LENGTH             = 180;
float constexpr GOAL_TIME                = 1.8f;
float constexpr CAMERA_SHAKE_MAGNITUDE_X = 20.f;
float constexpr CAMERA_SHAKE_MAGNITUDE_Y = 10.f;
float constexpr CAMERA_SHAKE_DURATION    = 0.5f;

MainState::MainState(GameStateManager* gameStateManager, Game* game, tgui::Gui* gui, int playerCount) :
GameState(gameStateManager, game, gui),
m_spriteManager(game->getWindow()),
m_players()
{
    initGui();

    for (int i = 0; i < playerCount; ++i)
        m_players.push_back(std::make_shared<Player>(i, m_game->getWindow()));

    const auto goalListenerId = EventBus::getInstance()
                                    .addListener(GoalEvent::Type,
                                                 [this](const IEvent::Ptr& event)
                                                 {
                                                     const auto goalEvent = std::static_pointer_cast<GoalEvent>(event);
                                                     int        playerIndex = goalEvent->getData();
                                                     handleGoal(playerIndex);
                                                 });
    m_listeners.push_back(goalListenerId);

    const auto goalListenerCameraShake = EventBus::getInstance().addListener(GoalEvent::Type,
                                                                             [this](const IEvent::Ptr& event)
                                                                             { activateCameraShake(); });
    m_listeners.push_back(goalListenerId);

    const auto resizeListenerId = EventBus::getInstance().addListener(ResizeEvent::Type,
                                                                      [this](const IEvent::Ptr& event)
                                                                      { updateCamera(); });
    m_listeners.push_back(resizeListenerId);
}

void MainState::initGui()
{
    auto        guiGroup = tgui::Group::create();
    std::string filename = "matchUI.txt";
    guiGroup->loadWidgetsFromFile(Config::guiPath + filename);
    m_guiGroups.emplace("Scoreboard", guiGroup);
    m_gui->add(guiGroup);
    m_guiGroups.at("Scoreboard")->setVisible(false);

    guiGroup = tgui::Group::create();
    filename = "goal.txt";
    guiGroup->loadWidgetsFromFile(Config::guiPath + filename);
    m_guiGroups.emplace("Goal", guiGroup);
    m_gui->add(guiGroup);
    m_guiGroups.at("Goal")->setVisible(false);
}

void MainState::init()
{
    PROFILE_FUNCTION();

    m_guiGroups.at("Scoreboard")->setVisible(true);
    m_guiGroups.at("Scoreboard")->get<tgui::Label>("Score1")->setText(tgui::String(0));
    m_guiGroups.at("Scoreboard")->get<tgui::Label>("Score2")->setText(tgui::String(0));
    m_timerSeconds = ROUND_LENGTH;
    m_accumulator  = 0.f;
    updateTimer(0);

    m_gameObjectManager.init();
    m_spriteManager.init();
    m_physicsManager.init();
    loadAssets();

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

    m_cameraShake = std::make_shared<TransformAnimationCameraShake>(sf::Vector2f(CAMERA_SHAKE_MAGNITUDE_X,
                                                                                 CAMERA_SHAKE_MAGNITUDE_Y),
                                                                    CAMERA_SHAKE_DURATION);
    camera->addComponent<TransformAnimationComponent>(*camera, m_cameraShake);

    if (!camera->init())
        FF_ERROR_MSG("Could not initialize camera");

    m_gameObjectManager.addGameObject(camera);
    m_spriteManager.setCamera(renderComponent.get());


    // define layers
    m_spriteManager.setLayerOrder({"Background", "GameObjects"});

    m_scored = false;

    for (auto& p : m_players)
        p->startMatch();
}

void MainState::update(const float deltaTime)
{
    PROFILE_FUNCTION();
    if (InputManager::getInstance().isActionPressed("Exit"))
    {
        m_gameStateManager->setState("MenuState");
        return;
    }

    EventBus::getInstance().processEvents(deltaTime);
    m_gameObjectManager.update(deltaTime);
    m_physicsManager.update(deltaTime);

    updateTimer(deltaTime);
    m_goalHandler.update(deltaTime);
    for (auto& p : m_players)
        p->update(deltaTime);
}

void MainState::updateTimer(const float deltaTime)
{
    m_accumulator += deltaTime;
    while (m_accumulator > 1.0f)
    {
        m_timerSeconds--;

        int         seconds    = m_timerSeconds % 60;
        int         minutes    = m_timerSeconds / 60;
        std::string seperation = (seconds > 9) ? " : " : " : 0";
        auto        time       = tgui::String(minutes) + seperation + tgui::String(seconds);
        m_guiGroups.at("Scoreboard")->get<tgui::Label>("Timer")->setText(time);

        m_accumulator--;
    }

    if (m_scored)
    {
        if (m_goalTime < GOAL_TIME)
            m_goalTime += deltaTime;
        else
        {
            m_scored = false;
            m_guiGroups.at("Goal")->setVisible(false);
        }
    }

    if (m_timerSeconds < 1)
    {
        exit();
        init();
    }
}

void MainState::handleGoal(int playerIndex)
{
    if (playerIndex < 1 || playerIndex > 2)
        std::cout << "Scoring player non existent" << std::endl;

    int currScore = 0;
    if (playerIndex == 1)
    {
        currScore = stoi(m_guiGroups.at("Scoreboard")->get<tgui::Label>("Score2")->getText().toStdString());
        currScore++;
        m_guiGroups.at("Scoreboard")->get<tgui::Label>("Score2")->setText(tgui::String(currScore));
    }
    else if (playerIndex == 2)
    {
        currScore = stoi(m_guiGroups.at("Scoreboard")->get<tgui::Label>("Score1")->getText().toStdString());
        currScore++;
        m_guiGroups.at("Scoreboard")->get<tgui::Label>("Score1")->setText(tgui::String(currScore));
    }
    m_scored   = true;
    m_goalTime = 0.f;
    m_guiGroups.at("Goal")->setVisible(true);

    if (currScore > 9)
    {
        exit();
        init();
    }
}

void MainState::draw()
{
    PROFILE_FUNCTION();
    m_spriteManager.draw();
}

void MainState::exit()
{
    PROFILE_FUNCTION();
    for (auto& group : m_guiGroups)
        group.second->setVisible(false);

    for (auto it = m_listeners.begin(); it != m_listeners.end(); ++it)
    {
        EventBus::getInstance().removeListener(*it);
    }
    m_listeners.clear();

    m_physicsManager.shutdown();
    m_spriteManager.shutdown();
    m_gameObjectManager.shutdown();
    for (auto p : m_players)
        p->shutdown();
}

void MainState::activateCameraShake()
{
    try
    {
        auto ptr = std::dynamic_pointer_cast<TransformAnimationCameraShake>(m_cameraShake);
        if (ptr)
        {
            ptr->activate();
        }
    } catch (std::exception e)
    {
        std::cerr << "ERROR: Couldn't activate camera shake" << std::endl;
    }
}

void MainState::updateCamera()
{
    auto camera = m_gameObjectManager.getGameObject("Camera");
    camera->removeComponent(camera->getComponent<CameraRenderComponent>());
    const auto renderComponent = camera->addComponent<CameraRenderComponent>(*camera, m_game->getWindow(), m_game->getWindow().getView());
    m_spriteManager.setCamera(renderComponent.get());
}

void MainState::loadAssets()
{
    AssetManager::getInstance().loadTexture("Selected Marker", "selected_marker.png");
}
} // namespace mmt_gd
