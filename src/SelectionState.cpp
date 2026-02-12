#include "stdafx.h"

#include "SelectionState.hpp"

#include "EventBus.hpp"
#include "Game.hpp"
#include "GameStateManager.hpp"
#include "InputManager.hpp"
#include "PlayerConfigEvent.hpp"

namespace mmt_gd
{
void SelectionState::initDefaultConfigs()
{
    m_configs.at(0) = PlayerConfig{};
    m_configs.at(1) = PlayerConfig{};

    for (unsigned int i = 0; i < m_configs.size(); ++i)
    {
        m_configs[i].m_playerIndex = i;
        auto& paddles = m_configs[i].m_config;
        for (unsigned int j = 0; j < paddles.size(); j++)
        {
            auto& paddle = paddles[j];
            paddle.m_paddleIndex = i * paddles.size() + j;
            paddle.m_ability     = PaddleAbility::DASH;
            paddle.m_passive     = PaddlePassive::NOTHING;
        }
    }
}

void SelectionState::initGui()
{
    auto guiGroup = tgui::Group::create();
    guiGroup->loadWidgetsFromFile(Config::guiPath + "setup_draft.txt");
    m_guiGroups.emplace("Setup", guiGroup);
    m_gui->add(guiGroup);
    m_guiGroups.at("Setup")->setVisible(true);

    int paddleNum = m_configs.at(0).m_config.size();
    for (unsigned int i = 0; i < m_configs.size(); ++i)
        initPlayerGui(i, paddleNum);
}

void SelectionState::initPlayerGui(int playerIndex, int playerPaddles)
{
    auto& guiGroup = m_guiGroups.at("Setup");
    for (int i = 0; i < playerPaddles; ++i)
    {
        auto idx = tgui::String(playerPaddles*playerIndex + (i + 1));
        m_panels.at(playerIndex).at(i) = guiGroup->get<tgui::Panel>("Panel" + tgui::String(idx));
        auto subGroup = guiGroup->get<tgui::Group>("Group" + idx);
        if (subGroup)
        {
            m_labels.at(playerIndex).at(2 * i)     = subGroup->get<tgui::Label>("Active" + tgui::String(idx));
            m_labels.at(playerIndex).at(2 * i)->getRenderer()->setTextColor(tgui::Color::Black);
            m_labels.at(playerIndex).at(2 * i + 1) = subGroup->get<tgui::Label>("Passive" + tgui::String(idx));
            m_labels.at(playerIndex).at(2 * i + 1)->getRenderer()->setTextColor(tgui::Color::Black);
        }
        m_labels.at(playerIndex).at(2*playerPaddles) = guiGroup->get<tgui::Label>("Done" + tgui::String(playerIndex+1));
    }
}

void SelectionState::initSprites()
{
    auto panelSize = m_panels.at(0).at(0)->getSize();
    auto& texture = AssetManager::getInstance().getTexture("PaddleBase");
    auto scale = sf::Vector2f(0.75*panelSize.x / texture.getSize().x, 0.75*panelSize.y / texture.getSize().y);
    auto  origin = sf::Vector2f(texture.getSize().x / 2.f, texture.getSize().y / 2.f);

    for (unsigned int i = 0; i < m_sprites.size(); ++i)
    {
        //m_previewShader->setUniform("team", i);
        for (unsigned int j = 0; j < m_sprites.at(i).size(); ++j)
        {
            auto& panel = m_panels.at(i).at(j);
            sf::Sprite sprite;
            sprite.setTexture(texture);
            sprite.setTextureRect(sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y));
            sprite.setOrigin(origin);
            sprite.setScale(scale);
            sf::Vector2f pos = panel->getAbsolutePosition();
            pos += sf::Vector2f(panel->getSize().x/2.f, panel->getSize().y/2.f);
            sprite.setPosition(pos);
            m_sprites.at(i).at(j) = sprite;

            std::cout << std::endl;
            std::cout << sprite.getPosition().x << " | " << sprite.getPosition().y << std::endl;
        }
    }

    m_previewShader = AssetManager::getInstance().getFragmentShader("PreviewShader");
    m_previewShader->setUniform("palette", AssetManager::getInstance().getTexture("PreviewPalette"));
    m_previewShader->setUniform("team", 0);
    m_previewShader->setUniform("ability", 0);
    m_previewShader->setUniform("passive", 0);

    m_guiGroups.at("Setup")->setVisible(false);
}

void SelectionState::init()
{
    PROFILE_FUNCTION();
    //m_guiGroup->setVisible(true);

    if (m_isInit)
    {
        return;
    }

    initDefaultConfigs();
    initGui();
    initSprites();
    m_focusIndex.fill(0);
    m_lastFocus.fill(0);
    m_playerReady.fill(false);

    m_isInit = true;
}

void SelectionState::update(float deltaTime)
{
    processPlayerInput(0);
    processPlayerInput(1);

    if (m_playerReady[0] == true && m_playerReady[1] == true)
        startMatch();
}

void SelectionState::processPlayerInput(int playerIndex)
{
    m_lastFocus.at(playerIndex) = m_focusIndex.at(playerIndex);

    if (m_playerReady.at(playerIndex))
    {
        if (InputManager::getInstance().isActionJustPressed("Pause", playerIndex))
            setReadyStatus(playerIndex, false);
        return;
    }

    if (InputManager::getInstance().isActionJustPressed("up", playerIndex))
        if (m_focusIndex.at(playerIndex) > 0)
            m_focusIndex.at(playerIndex)--;
    if (InputManager::getInstance().isActionJustPressed("down", playerIndex))
        if (m_focusIndex.at(playerIndex) < static_cast<int>(m_labels.at(playerIndex).size()-1))
            m_focusIndex.at(playerIndex)++;

    if (InputManager::getInstance().isActionJustPressed("left", playerIndex))
        updateConfig(playerIndex, false);
    if (InputManager::getInstance().isActionJustPressed("right", playerIndex))
        updateConfig(playerIndex, true);
    if (m_focusIndex.at(playerIndex) == m_labels.at(playerIndex).size() - 1)
    {
        if (InputManager::getInstance().isActionJustPressed("switch", playerIndex) && !m_playerReady.at(playerIndex))
            setReadyStatus(playerIndex, true);
        else if ((InputManager::getInstance().isActionJustPressed("switch", playerIndex)
            || InputManager::getInstance().isActionJustPressed("dash", playerIndex))
            && m_playerReady.at(playerIndex))
             setReadyStatus(playerIndex, false);
    }

    updateFocus(playerIndex);
}

void SelectionState::updateFocus(int playerIndex)
{
    int  oldFocus = m_lastFocus.at(playerIndex);
    int  newFocus = m_focusIndex.at(playerIndex);
    auto oldRend  = m_labels.at(playerIndex).at(oldFocus)->getRenderer();
    auto newRend  = m_labels.at(playerIndex).at(newFocus)->getRenderer();

    oldRend->setTextColor(tgui::Color::Black);
    newRend->setTextColor(tgui::Color::Blue);
}

void SelectionState::setReadyStatus(int playerIndex, bool status)
{
    m_playerReady.at(playerIndex) = true;
    int idx = m_focusIndex.at(playerIndex);
    auto rend = m_labels.at(playerIndex).at(idx)->getRenderer();

    if (status)
        rend->setBackgroundColor(tgui::Color::Green);
    else
        rend->setBackgroundColor(tgui::Color::Transparent);
}

void SelectionState::updateConfig(int playerIndex, bool right)
{
    int idx = m_focusIndex.at(playerIndex);
    if (idx == 6)
        return;

    int half = idx / 2;
    PaddleConfig& conf = m_configs.at(playerIndex).m_config.at(half);

    // focus is Active
    if (idx % 2 == 0)
    {
        conf.m_ability = PaddleConfig::GetNextActive(conf.m_ability);
        auto text = tgui::String(PaddleConfig::ActiveToString(conf.m_ability));
        m_labels.at(playerIndex).at(idx)->setText(text);
        // add shader update
    }

    //focus is Passive
    else
    {
        conf.m_passive = PaddleConfig::GetNextPassive(conf.m_passive);
        auto text = tgui::String(PaddleConfig::PassiveToString(conf.m_passive));
        m_labels.at(playerIndex).at(idx)->setText(text);
        // add shader update
    }
}

void SelectionState::draw()
{
    m_guiGroups.at("Setup")->setVisible(true);
    sf::RenderWindow& window = m_game->getWindow();

    m_gui->draw();
    for (unsigned int i = 0; i < m_sprites.size(); ++i)
        for (unsigned int j = 0; j < m_sprites.at(i).size(); ++j)
            drawPaddle(i, j, window);

    m_guiGroups.at("Setup")->setVisible(false);
}

void SelectionState::drawPaddle(int playerIndex, int paddleIndex, sf::RenderWindow& window)
{
    auto& config = m_configs.at(playerIndex).m_config.at(paddleIndex);
    int active = static_cast<int>(config.m_ability);
    int passive = static_cast<int>(config.m_passive);

    m_previewShader->setUniform("team", playerIndex);
    //m_previewShader->setUniform("paddle", paddleIndex);
    m_previewShader->setUniform("ability", active);
    m_previewShader->setUniform("passive", passive);

    window.draw(m_sprites.at(playerIndex).at(paddleIndex), m_previewShader);
}

void SelectionState::exit()
{
    m_isInit = false;

    for (auto& group : m_guiGroups)
    {
        m_gui->remove(group.second);
    }
    m_guiGroups.clear();


    m_lastFocus.fill(0);
    m_focusIndex.fill(0);
    m_playerReady.fill(false);
    m_configs = {PlayerConfig{}, PlayerConfig{}};

    for (auto& arr : m_labels)
        arr.fill(nullptr);

    for (auto& arr : m_panels)
        arr.fill(nullptr);

    for (auto& arr : m_sprites)
        arr.fill(sf::Sprite());
}

void SelectionState::startMatch()
{
    EventBus::getInstance().fireEvent(std::make_shared<PlayerConfigFinishEvent>(m_configs.at(0)));
    EventBus::getInstance().fireEvent(std::make_shared<PlayerConfigFinishEvent>(m_configs.at(1)));

    m_gameStateManager->setState("MainState");
}

void SelectionState::loadAssets()
{
    AssetManager::getInstance().loadFragmentShader("PreviewShader", "PreviewShader.frag");
    AssetManager::getInstance().loadTexture("PaddleBase", "PaddleBase.png");
    AssetManager::getInstance().loadTexture("PreviewPalette", "PreviewPalette.png");
    AssetManager::getInstance().getTexture("PreviewPalette").setSmooth(false);
}

}
