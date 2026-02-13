//This code was made for the Multimedia Project 2a for the Multimedia Technology class at Fachhochschule Salzburg, by Pascal Veselka & Pascal Marsche
#pragma once

#include "AssetManager.hpp"
#include "GameState.hpp"
#include "PlayerConfig.hpp"


namespace mmt_gd
{
class GameStateManager;
class Game;

class SelectionState : public GameState
{
public:
    SelectionState(GameStateManager* gameStateManager, Game* game, tgui::Gui* gui) :
    GameState(gameStateManager, game, gui)
    {
        loadAssets();
        m_previewShader = AssetManager::getInstance().getFragmentShader("PreviewShader");
        m_configs       = {PlayerConfig{}, PlayerConfig{}};
        m_focusIndex.fill(0);
        m_lastFocus.fill(0);
        m_playerReady.fill(false);
    }

    virtual void init() override;
    virtual void exit() override;

    virtual void update(float delta) override;
    virtual void draw() override;
    virtual void initGui() override;

private:
    void loadAssets();
    void initSprites();
    void initDefaultConfigs();
    void initPlayerGui(int playerIndex, int playerPaddles);
    void processPlayerInput(int playerIndex);
    void updateConfig(int playerIndex, bool right);
    void updateFocus(int playerIndex);
    void setReadyStatus(int playerIndex, bool status);
    void drawPaddle(int playerIndex, int paddleIndex, sf::RenderWindow& window);
    void startMatch();

    std::array<int, 2>          m_lastFocus;
    std::array<int, 2>          m_focusIndex;
    std::array<bool, 2>         m_playerReady;
    std::array<PlayerConfig, 2> m_configs;

    std::array<std::array<tgui::Label::Ptr, 7>, 2> m_labels;
    std::array<std::array<tgui::Panel::Ptr, 3>, 2> m_panels;

    std::array<std::array<sf::Sprite, 3>, 2> m_sprites;
    sf::Shader*                              m_previewShader;

    sf::Vector2i m_paletteSize;
};
}; // namespace mmt_gd