#include "stdafx.h"

#include "PaddleSetupState.hpp"

#include "EventBus.hpp"
#include "Game.hpp"
#include "GameStateManager.hpp"
#include "InputManager.hpp"
#include "PlayerConfigEvent.hpp"

namespace mmt_gd
{
using namespace std;

void PaddleSetupState::init()
{
    PROFILE_FUNCTION();
    //m_guiGroup->setVisible(true);

    if (m_isInit)
    {
        return;
    }

    m_isInit = true;

    // LoadGui
    auto guiGroup = tgui::Group::create();
    guiGroup->loadWidgetsFromFile(Config::guiPath + "paddle_setup.txt");
    m_guiGroups.emplace("Setup", guiGroup);
    m_gui->add(guiGroup);
    m_guiGroups.at("Setup")->setVisible(true);

    auto player1 = guiGroup->get("Player1");
    auto player2 = guiGroup->get("Player2");
    if (const auto p1 = dynamic_pointer_cast<tgui::Group>(player1))
    {
        for (auto w : p1->getWidgets())
        {
            if (const auto box = dynamic_pointer_cast<tgui::ListBox>(w))
            {
                if (box->getWidgetName().toStdString() == "Active1")
                    m_player1Active = box;
                else
                    m_player1Inactive = box;
            }
        }
    }
    if (const auto p2 = dynamic_pointer_cast<tgui::Group>(player2))
    {
        for (auto w : p2->getWidgets())
        {
            if (const auto box = dynamic_pointer_cast<tgui::ListBox>(w))
            {
                if (box->getWidgetName().toStdString() == "Active2")
                    m_player2Active = box;
                else
                    m_player2Inactive = box;
            }
        }
    }

    m_player1Progress = SetupStep::Active;
    m_player2Progress = SetupStep::Active;
}

void PaddleSetupState::update(float delta)
{
    PROFILE_FUNCTION();

    handleButtons1();
    handleButtons2();

    if (m_player1Progress == SetupStep::Finished && m_player2Progress == SetupStep::Finished)
    {
        sendEvent();
        m_gameStateManager->setState("MainState");
    }
}

void PaddleSetupState::draw()
{
    PROFILE_FUNCTION();
}

void PaddleSetupState::exit()
{
    PROFILE_FUNCTION();

    m_isInit = false;
    GameState::exit();
}

void PaddleSetupState::handleButtons1()
{
    if (InputManager::getInstance().isActionJustPressed("down", 0))
    {
        if (m_player1Progress == SetupStep::Active)
        {
            int curr = m_player1Active->getSelectedItemIndex();
            m_player1Active->deselectItem();
            int maxIdx = m_player1Active->getItemCount() - 1;
            curr = curr < maxIdx ? curr + 1 : 0;
            m_player1Active->setSelectedItemByIndex(curr);
        }
        else if (m_player1Progress == SetupStep::Passive)
        {
            int curr = m_player1Inactive->getSelectedItemIndex();
            m_player1Inactive->deselectItem();
            int maxIdx = m_player1Inactive->getItemCount() - 1;
            curr = curr < maxIdx ? curr + 1 : 0;
            m_player1Inactive->setSelectedItemByIndex(curr);
        }
    }
    else if (InputManager::getInstance().isActionJustPressed("up", 0))
    {
        if (m_player1Progress == SetupStep::Active)
        {
            int curr = m_player1Active->getSelectedItemIndex();
            m_player1Active->deselectItem();
            curr = curr > 0 ? curr - 1 : m_player1Active->getItemCount() - 1;
            m_player1Active->setSelectedItemByIndex(curr);
        }
        else if (m_player1Progress == SetupStep::Passive)
        {
            int curr = m_player1Inactive->getSelectedItemIndex();
            m_player1Inactive->deselectItem();
            curr = curr > 0 ? curr - 1 : m_player1Inactive->getItemCount() - 1;
            m_player1Inactive->setSelectedItemByIndex(curr);
        }
    }
    if (InputManager::getInstance().isActionJustPressed("switch", 0))
    {
        if (m_player1Progress != SetupStep::Finished)
            m_player1Progress = (SetupStep)(((int)m_player1Progress) + 1);
    }
    else if (InputManager::getInstance().isActionJustPressed("ability", 0))
    {
        if (m_player1Progress != SetupStep::Active)
            m_player1Progress = (SetupStep)(((int)m_player1Progress) - 1);
    }
}

void PaddleSetupState::handleButtons2()
{
    if (InputManager::getInstance().isActionJustPressed("down", 1))
    {
        if (m_player2Progress == SetupStep::Active)
        {
            int curr = m_player2Active->getSelectedItemIndex();
            m_player2Active->deselectItem();
            int maxIdx = m_player2Active->getItemCount() - 1;
            curr = curr < maxIdx ? curr + 1 : 0;
            m_player2Active->setSelectedItemByIndex(curr);
        }
        else if (m_player2Progress == SetupStep::Passive)
        {
            int curr = m_player2Inactive->getSelectedItemIndex();
            m_player2Inactive->deselectItem();
            int maxIdx = m_player1Inactive->getItemCount() - 1;
            curr = curr < maxIdx ? curr + 1 : 0;
            m_player2Inactive->setSelectedItemByIndex(curr);
        }
    }
    else if (InputManager::getInstance().isActionJustPressed("up", 1))
    {
        if (m_player2Progress == SetupStep::Active)
        {
            int curr = m_player2Active->getSelectedItemIndex();
            m_player2Active->deselectItem();
            curr = curr > 0 ? curr - 1 : m_player2Active->getItemCount() - 1;
            m_player2Active->setSelectedItemByIndex(curr);
        }
        else if (m_player2Progress == SetupStep::Passive)
        {
            int curr = m_player2Inactive->getSelectedItemIndex();
            m_player2Inactive->deselectItem();
            curr = curr > 0 ? curr - 1 : m_player2Inactive->getItemCount() - 1;
            m_player2Inactive->setSelectedItemByIndex(curr);
        }
    }
    else if (InputManager::getInstance().isActionJustPressed("switch", 1))
    {
        if (m_player2Progress != SetupStep::Finished)
            m_player2Progress = (SetupStep)(((int)m_player2Progress) + 1);
    }
    else if (InputManager::getInstance().isActionJustPressed("ability", 1))
    {
        if (m_player2Progress != SetupStep::Active)
            m_player2Progress = (SetupStep)(((int)m_player2Progress) - 1);
    }
}

void PaddleSetupState::sendEvent()
{
    PlayerConfig config1;
    PlayerConfig config2;
    config1.m_playerIndex = 0;
    config2.m_playerIndex = 1;

    PaddleAbility ability1;
    if (m_player1Active->getSelectedItem().toStdString() == "Enlarge")
        ability1 = PaddleAbility::ENLARGE;
    else    
        ability1 = PaddleAbility::DASH;

    PaddleAbility ability2;
    if (m_player2Active->getSelectedItem().toStdString() == "Enlarge")
        ability2 = PaddleAbility::ENLARGE;
    else
        ability2 = PaddleAbility::DASH;

    PaddlePassive passive1;
    if (m_player1Inactive->getSelectedItem().toStdString() == "Heavy")
        passive1 = PaddlePassive::KILL;
    else if (m_player1Inactive->getSelectedItem().toStdString() == "Bouncy")
        passive1 = PaddlePassive::BOUNCY;
    else
        passive1 = PaddlePassive::NOTHING;

    PaddlePassive passive2;
    if (m_player2Inactive->getSelectedItem().toStdString() == "Heavy")
        passive2 = PaddlePassive::KILL;
    else if (m_player2Inactive->getSelectedItem().toStdString() == "Bouncy")
        passive2 = PaddlePassive::BOUNCY;
    else
        passive2 = PaddlePassive::NOTHING;

    for (int i = 0; i < 3; ++i)
    {
        config1.m_config.at(i).m_ability     = ability1;
        config1.m_config.at(i).m_passive     = passive1;
        config1.m_config.at(i).m_paddleIndex = i;
    }

    for (int i = 0; i < 3; ++i)
    {
        config2.m_config.at(i).m_ability     = ability2;
        config2.m_config.at(i).m_passive     = passive2;
        config2.m_config.at(i).m_paddleIndex = i;
    }

    EventBus::getInstance().fireEvent(std::make_shared<PlayerConfigFinishEvent>(config1));
    EventBus::getInstance().fireEvent(std::make_shared<PlayerConfigFinishEvent>(config2));
}
} // namespace mmt_gd
