#include "stdafx.h"

#include "MenuState.hpp"

#include "Game.hpp"
#include "GameStateManager.hpp"
#include "InputManager.hpp"

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

    auto buttonGroup = m_game->getGui().get("Buttons");
    if (const auto grp = dynamic_pointer_cast<tgui::Group>(buttonGroup))
    {
        int cnt = 0;
        for (auto w : grp->getWidgets())
        {
            if (cnt == m_buttons.max_size())
                break;
            if (const auto btn = dynamic_pointer_cast<tgui::Button>(w))
            {
                m_buttons[cnt] = btn;
                cnt++;
                std::string name            = btn->getWidgetName().toStdString();

                if (name == "Start")
                    btn->onPress([&manager = m_gameStateManager] { manager->setState("MainState"); });
                else if (name == "Settings")
                    continue;
                //btn->onPress([&manager = m_gameStateManager] { manager->setState("Settings"); });
                else if (name == "Quit")
                    btn->onPress([&game = m_game] { game->getWindow().close(); });
            }
        }
        m_selectedButton = 0;
        m_buttons[m_selectedButton]->setFocused(true);
    }

    // SIGNALLLLL??????
}

void MenuState::update(float delta)
{
    PROFILE_FUNCTION();

    m_game->getWindow().setView(m_view);

    handleButtons();
}

void MenuState::draw()
{
    PROFILE_FUNCTION();
}

void MenuState::exit()
{
    PROFILE_FUNCTION();

    m_game->getGui().removeAllWidgets();
    m_isInit = false;
    GameState::exit();
}

void MenuState::handleButtons()
{
    int prevSelected = m_selectedButton;
    if (InputManager::getInstance().isActionJustPressed("down") 
        || InputManager::getInstance().isActionJustPressed("right"))
        m_selectedButton++;
    else if (InputManager::getInstance().isActionJustPressed("up") ||
             InputManager::getInstance().isActionJustPressed("left"))
        m_selectedButton--;
    else if (InputManager::getInstance().isActionJustPressed("switch"))
        std::cout << "press" << std::endl;

    if (prevSelected != m_selectedButton)
    {
        m_selectedButton = m_selectedButton < 0 ? m_buttons.size() - 1 : m_selectedButton % m_buttons.size();
        m_buttons[prevSelected]->setFocused(false);
        m_buttons[m_selectedButton]->setFocused(true);
    }

}
} // namespace mmt_gd
