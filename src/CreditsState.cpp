#include "stdafx.h"

#include "CreditsState.hpp"

#include "Game.hpp"
#include "GameStateManager.hpp"
#include "InputManager.hpp"

namespace mmt_gd
{
using namespace std;

const std::string UI_GROUP_NAME = "Credits";

void CreditsState::init()
{
    PROFILE_FUNCTION();
    //m_guiGroup->setVisible(true);

    if (m_isInit)
    {
        m_guiGroups[UI_GROUP_NAME]->setVisible(true);
        m_backButton->setFocused(true);
        return;
    }

    m_view = m_game->getWindow().getView();

    m_isInit = true;

    // LoadGui
    auto guiGroup = tgui::Group::create();
    guiGroup->loadWidgetsFromFile(Config::guiPath + "credits.txt");
    m_guiGroups.emplace(UI_GROUP_NAME, guiGroup);
    m_gui->add(guiGroup);
    m_guiGroups[UI_GROUP_NAME]->setVisible(true);

    m_backButton = guiGroup->get("Back");
    m_backButton->getSignal("Pressed").connect([&manager = m_gameStateManager]() { manager->setState("MenuState"); });
    m_backButton->setFocused(true);
}

void CreditsState::update(float delta)
{
    PROFILE_FUNCTION();

    m_game->getWindow().setView(m_view);

    if (InputManager::getInstance().isActionJustPressed("switch"))
        m_backButton->getSignal("Pressed").emit(m_backButton.get());
}

void CreditsState::draw()
{
    PROFILE_FUNCTION();
}

void CreditsState::exit()
{
    PROFILE_FUNCTION();
    GameState::exit();
}
} // namespace mmt_gd
