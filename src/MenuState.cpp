#include "stdafx.h"

#include "MenuState.hpp"

#include "Game.hpp"
#include "GameStateManager.hpp"
#include "InputManager.hpp"
#include "TGUI/TGUI.hpp"


namespace mmt_gd
{
using namespace std;

void MenuState::init()
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
    m_game->getGui().loadWidgetsFromFile("../assets/main_menu.txt");

    if (const auto btn = dynamic_pointer_cast<tgui::Button>(m_game->getGui().get("Start")))
    {
        btn->onClick([&manager = m_gameStateManager] { manager->setState("MainState"); });
    }
}

void MenuState::update(float delta)
{
    PROFILE_FUNCTION();

    m_game->getWindow().setView(m_view);

    if (InputManager::getInstance().isActionPressed("Select"))
    {
        m_gameStateManager->setState("MainState");
    }
}

void MenuState::draw()
{
    PROFILE_FUNCTION();
}

void MenuState::exit()
{
    PROFILE_FUNCTION();

    for (auto& group : m_guiGroups)
        group.second->setVisible(false);

    //m_game->getGui().removeAllWidgets();
    GameState::exit();
}
} // namespace mmt_gd
