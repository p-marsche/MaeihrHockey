#pragma once

#include "GameObjectManager.hpp"
#include "GameState.hpp"
#include "PhysicsManager.hpp"
#include "SpriteManager.hpp"
#include "GoalHandler.hpp"
#include "Player.hpp"

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

    void initGui() override;
    void updateTimer(const float deltaTime);
    void handleGoal(int playerIndex);

private:
    SpriteManager                           m_spriteManager;
    GameObjectManager                       m_gameObjectManager;
    PhysicsManager                          m_physicsManager;
    GoalHandler                             m_goalHandler;
    std::vector<std::shared_ptr<Player>>    m_players;
    int                                     m_timerSeconds;
    float                                   m_accumulator, m_goalTime;
    std::list<mmt_gd::EventBus::ListenerId> m_listeners;
    bool                                    m_scored;

};
} // namespace mmt_gd
