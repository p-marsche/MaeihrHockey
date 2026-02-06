#include "stdafx.h"

#include "PauseState.hpp"

#include "Game.hpp"
#include "GameStateManager.hpp"
#include "InputManager.hpp"

namespace mmt_gd
{
using namespace std;

void PauseState::init()
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
                m_buttons[cnt] = w;
                cnt++;
                std::string name = btn->getWidgetName().toStdString();

                if (name == "Continue")
                    w->getSignal("Pressed").connect([&manager = m_gameStateManager] { manager->setState("MainState"); });
                else if (name == "Main Menu")
                    w->getSignal("Pressed").connect(
                        [&manager = m_gameStateManager] { manager->setState("MenuState"); });
                //btn->onPress([&manager = m_gameStateManager] { manager->setState("Settings"); });
                else if (name == "Quit")
                    w->getSignal("Pressed").connect([&game = m_game] { game->getWindow().close(); });
            }
        }
        m_selectedButton = 0;
        m_buttons[m_selectedButton]->setFocused(true);
    }
}

void PauseState::update(float delta)
{
    PROFILE_FUNCTION();

    m_game->getWindow().setView(m_view);

    handleButtons();
}

void PauseState::draw()
{
    PROFILE_FUNCTION();
}

void PauseState::exit()
{
    PROFILE_FUNCTION();

    m_game->getGui().removeAllWidgets();
    m_isInit = false;
    GameState::exit();
}

void PauseState::handleButtons()
{
    int prevSelected = m_selectedButton;
    if (InputManager::getInstance().isActionJustPressed("down") ||
        InputManager::getInstance().isActionJustPressed(
            "rig"
            "h"
            "t"))
        m_selectedButton++;
    else if (InputManager::getInstance().isActionJustPressed("up") ||
             InputManager::getInstance().isActionJustPressed("left"))
        m_selectedButton--;
    else if (InputManager::getInstance().isActionJustPressed("switch"))
    {
        auto widget = m_buttons[m_selectedButton];
        widget->getSignal("Pressed").emit(widget.get());
    }
    else if (InputManager::getInstance().isActionJustPressed("Pause"))
        m_gameStateManager->setState("MainState");

    if (prevSelected != m_selectedButton)
    {
        m_selectedButton = m_selectedButton < 0 ? m_buttons.size() - 1 : m_selectedButton % m_buttons.size();
        m_buttons[prevSelected]->setFocused(false);
        m_buttons[m_selectedButton]->setFocused(true);
    }
}
} // namespace mmt_gd
