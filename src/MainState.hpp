#pragma once

#include "GameObjectManager.hpp"
#include "GameState.hpp"
#include "GoalHandler.hpp"
#include "ITransformAnimation.hpp"
#include "PhysicsManager.hpp"
#include "Player.hpp"
#include "SpriteManager.hpp"
#include "PlayerConfig.hpp"

namespace mmt_gd
{
class FINALFRONTIER_API MainState final : public GameState
{
public:
    MainState(GameStateManager* gameStateManager, Game* game, tgui::Gui* gui, int playerCount);

    void init() override;
    void exit() override;

    void update(float deltaTime) override;
    void draw() override;

    void addConfig(std::unordered_map<int, PlayerConfig> config)
    {
        m_config = config;
    }

private:
    void                                    initGui() override;
    void                                    updateTimer(const float deltaTime);
    void                                    handleGoal(int playerIndex);
    void                                    activateCameraShake();
    void                                    updateCamera();
    void                                    loadAssets();

    SpriteManager                           m_spriteManager;
    GameObjectManager                       m_gameObjectManager;
    PhysicsManager                          m_physicsManager;
    GoalHandler                             m_goalHandler;
    std::vector<std::shared_ptr<Player>>    m_players;
    int                                     m_timerSeconds;
    float                                   m_accumulator, m_goalTime;
    std::list<mmt_gd::EventBus::ListenerId> m_listeners;
    bool                                    m_scored;
    ITransformAnimation::Ptr                m_cameraShake;
    std::unordered_map<int, PlayerConfig>   m_config;
};
} // namespace mmt_gd
