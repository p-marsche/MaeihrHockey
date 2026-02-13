//This code was made for the Multimedia Project 2a for the Multimedia Technology class at Fachhochschule Salzburg, by Pascal Veselka & Pascal Marsche
#pragma once

#include "GameObjectManager.hpp"
#include "GameState.hpp"
#include "GoalHandler.hpp"
#include "ITransformAnimation.hpp"
#include "PhysicsManager.hpp"
#include "Player.hpp"
#include "PlayerConfig.hpp"
#include "SpriteManager.hpp"

namespace mmt_gd
{
class MainState final : public GameState
{
public:
    MainState(GameStateManager* gameStateManager, Game* game, tgui::Gui* gui, sf::Music& music, int playerCount);

    void init() override;
    void exit() override;
    void shutdown();

    void update(float deltaTime) override;
    void draw() override;

    void addConfig(PlayerConfig config);
    void enableGui() override;

private:
    void initGui() override;
    void updateTimer(const float deltaTime);
    void handleGoal(int playerIndex);
    void activateCameraShake();
    void updateCamera();
    void loadAssets();
    int  updateScore(int sideIndex);
    void pauseGame();
    void pauseLoop();

    SpriteManager                           m_spriteManager;
    GameObjectManager                       m_gameObjectManager;
    PhysicsManager                          m_physicsManager;
    GoalHandler                             m_goalHandler;
    std::vector<std::shared_ptr<Player>>    m_players;
    int                                     m_timerSeconds;
    float                                   m_accumulator, m_goalTime, m_endTimer;
    std::list<mmt_gd::EventBus::ListenerId> m_listeners;
    mmt_gd::EventBus::ListenerId            m_configListenerId;
    bool                                    m_scored;
    ITransformAnimation::Ptr                m_cameraShake;
    std::unordered_map<int, PlayerConfig>   m_config;
    sf::Music&                              m_music;
    sf::Sound                              m_goalSound;

    std::array<tgui::Widget::Ptr, 3> m_pauseButtons;
    int                              m_selectedButton;
    bool                             m_isPaused;

    int m_playerCount;
};
} // namespace mmt_gd
