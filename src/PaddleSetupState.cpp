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

    m_view = m_game->getWindow().getView();

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
                    player1Active = box;
                else
                    player1Inactive = box;
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
                    player2Active = box;
                else
                    player2Inactive = box;
            }
        }
    }

    player1ActiveSet = false;
    player2ActiveSet = false;
}

void PaddleSetupState::update(float delta)
{
    PROFILE_FUNCTION();

    handleButtons1();
    handleButtons2();

    m_game->getWindow().setView(m_view);
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
    if (InputManager::getInstance().isActionJustPressed("down", 0) ||
            InputManager::getInstance().isActionJustPressed("right", 0))
    {
        if (!player1ActiveSet)
        {
            int curr = player1Active->getSelectedItemIndex();
            player1Active->deselectItem();
            int maxIdx = player1Active->getItemCount() - 1;
            curr = curr < maxIdx ? curr + 1 : 0;
            player1Active->setSelectedItemByIndex(curr);
        }
        else
        {
            int curr = player1Inactive->getSelectedItemIndex();
            player1Inactive->deselectItem();
            int maxIdx = player1Inactive->getItemCount() - 1;
            curr = curr < maxIdx ? curr + 1 : 0;
            player1Inactive->setSelectedItemByIndex(curr);
        }
    }
    else if (InputManager::getInstance().isActionJustPressed("up", 0) ||
             InputManager::getInstance().isActionJustPressed("left", 0))
    {
        if (!player1ActiveSet)
        {
            int curr = player1Inactive->getSelectedItemIndex();
            player1Inactive->deselectItem();
            curr = curr > 0 ? curr - 1 : player1Inactive->getItemCount() - 1;
            player1Inactive->setSelectedItemByIndex(curr);
        }
        else
        {
            int curr = player1Inactive->getSelectedItemIndex();
            player1Inactive->deselectItem();
            curr = curr > 0 ? curr - 1 : player1Inactive->getItemCount() - 1;
            player1Inactive->setSelectedItemByIndex(curr);
        }
    }
    else if (InputManager::getInstance().isActionJustPressed("switch"), 0)
    {
        if (!player1ActiveSet)
            player1ActiveSet = true;
        else if (player1ActiveSet && player2ActiveSet)
            sendEvent();
    }
    else if (InputManager::getInstance().isActionJustPressed("ability"), 0)
    {
        player1ActiveSet = false;
    }
}

void PaddleSetupState::handleButtons2()
{
    if (InputManager::getInstance().isActionJustPressed("down", 1) ||
            InputManager::getInstance().isActionJustPressed("right"),
        1)
    {
        if (!player2ActiveSet)
        {
            int curr = player2Active->getSelectedItemIndex();
            player2Active->deselectItem();
            int maxIdx = player2Active->getItemCount() - 1;
            curr = curr < maxIdx ? curr + 1 : 0;
            player2Active->setSelectedItemByIndex(curr);
        }
        else
        {
            int curr = player2Inactive->getSelectedItemIndex();
            player2Inactive->deselectItem();
            int maxIdx = player1Inactive->getItemCount() - 1;
            curr = curr < maxIdx ? curr + 1 : 0;
            player2Inactive->setSelectedItemByIndex(curr);
        }
    }
    else if (InputManager::getInstance().isActionJustPressed("up", 1) ||
             InputManager::getInstance().isActionJustPressed("left", 1))
    {
        if (!player2ActiveSet)
        {
            int curr = player2Inactive->getSelectedItemIndex();
            player2Inactive->deselectItem();
            curr = curr > 0 ? curr - 1 : player2Inactive->getItemCount() - 1;
            player2Inactive->setSelectedItemByIndex(curr);
        }
        else
        {
            int curr = player2Inactive->getSelectedItemIndex();
            player2Inactive->deselectItem();
            curr = curr > 0 ? curr - 1 : player2Inactive->getItemCount() - 1;
            player2Inactive->setSelectedItemByIndex(curr);
        }
    }
    else if (InputManager::getInstance().isActionJustPressed("switch"), 1)
    {
        if (!player2ActiveSet)
            player2ActiveSet = true;
    }
    else if (InputManager::getInstance().isActionJustPressed("ability"), 1)
    {
        player2ActiveSet = false;
    }
}

void PaddleSetupState::sendEvent()
{
    PlayerConfig config1;
    PlayerConfig config2;
    config1.m_playerIndex = 0;
    config2.m_playerIndex = 1;

    PaddleAbility ability1;
    if (player1Active->getSelectedItem().toStdString() == "Enlarge")
        ability1 = PaddleAbility::ENLARGE;
    else    
        ability1 = PaddleAbility::DASH;

    PaddleAbility ability2;
    if (player2Active->getSelectedItem().toStdString() == "Enlarge")
        ability2 = PaddleAbility::ENLARGE;
    else
        ability2 = PaddleAbility::DASH;

    PaddlePassive passive1;
    if (player1Inactive->getSelectedItem().toStdString() == "Heavy")
        passive1 = PaddlePassive::KILL;
    else if (player1Inactive->getSelectedItem().toStdString() == "Bouncy")
        passive1 = PaddlePassive::BOUNCY;
    else
        passive1 = PaddlePassive::NOTHING;

    PaddlePassive passive2;
    if (player2Inactive->getSelectedItem().toStdString() == "Heavy")
        passive2 = PaddlePassive::KILL;
    else if (player2Inactive->getSelectedItem().toStdString() == "Bouncy")
        passive2 = PaddlePassive::BOUNCY;
    else
        passive2 = PaddlePassive::NOTHING;

    for (int i = 0; i < 3; ++i)
    {
        config1.m_config.at(i).m_ability     = ability1;
        config1.m_config.at(i).m_passive     = passive1;
        config1.m_config.at(i).m_paddleIndex = i;
    }

    for (int i = 0; i < 2; ++i)
    {
        config2.m_config.at(i).m_ability     = ability2;
        config2.m_config.at(i).m_passive     = passive2;
        config2.m_config.at(i).m_paddleIndex = i;
    }

    EventBus::getInstance().fireEvent(std::make_shared<PlayerConfigFinishEvent>(config1));
    EventBus::getInstance().fireEvent(std::make_shared<PlayerConfigFinishEvent>(config2));
}
} // namespace mmt_gd
