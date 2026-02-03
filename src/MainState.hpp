#pragma once

#include "GameObjectManager.hpp"
#include "GameState.hpp"
#include "PhysicsManager.hpp"
#include "SpriteManager.hpp"

namespace mmt_gd
{
class FINALFRONTIER_API MainState final : public GameState
{
public:
    MainState(GameStateManager* gameStateManager, Game* game, tgui::Gui* gui);

    void init() override;
    void exit() override;

    void update(float deltaTime) override;
    void draw() override;

    void initGui() override;
    void updateTimer(const float deltaTime);

private:
    SpriteManager     m_spriteManager;
    GameObjectManager m_gameObjectManager;
    PhysicsManager    m_physicsManager;
    int               m_timerSeconds;
    float             m_accumulator;
};
} // namespace mmt_gd
