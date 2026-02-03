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

private:
    SpriteManager     m_spriteManager;
    GameObjectManager m_gameObjectManager;
    PhysicsManager    m_physicsManager;
};
} // namespace mmt_gd
