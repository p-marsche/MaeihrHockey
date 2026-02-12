#include "stdafx.h"

#include "MenuState.hpp"

#include "Game.hpp"
#include "GameStateManager.hpp"
#include "InputManager.hpp"

namespace mmt_gd
{
using namespace std;

const std::string UI_GROUP_NAME = "MainMenu";

void MenuState::init()
{
    PROFILE_FUNCTION();
    //m_guiGroup->setVisible(true);

    if (m_isInit)
    {
        m_guiGroups[UI_GROUP_NAME]->setVisible(true);
        m_buttons[m_selectedButton]->setFocused(true);
        return;
    }

    m_isInit = true;

    m_selectedButton = 0;

    // LoadGui
    auto        guiGroup = tgui::Group::create();
    guiGroup->loadWidgetsFromFile(Config::guiPath + "main_menu.txt");
    m_guiGroups.emplace(UI_GROUP_NAME, guiGroup);
    m_gui->add(guiGroup);
    m_guiGroups[UI_GROUP_NAME]->setVisible(true);

    auto buttonGroup = guiGroup->get("Buttons");
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

                if (name == "Start")
                    w->getSignal("Pressed").connect([&manager = m_gameStateManager] { manager->setState("PaddleSetup"); });
               /* else if (name == "Settings")
                    w->getSignal("Pressed").connect(
                        [&manager = m_gameStateManager] { manager->setState("SettingsMenuState"); });*/
                //btn->onPress([&manager = m_gameStateManager] { manager->setState("Settings"); });
                else if (name == "Credits")
                    w->getSignal("Pressed").connect([&manager = m_gameStateManager] { manager->setState("Credits"); });
                else if (name == "Quit")
                    w->getSignal("Pressed").connect([&game = m_game] 
                        { 
                            game->shutdown();
                            game->getWindow().close(); 
                        });
            }
        }
        m_buttons[m_selectedButton]->setFocused(true);
    }
}

void MenuState::update(float delta)
{
    PROFILE_FUNCTION();

    handleButtons();
}

void MenuState::draw()
{
    PROFILE_FUNCTION();
}

void MenuState::exit()
{
    PROFILE_FUNCTION();

    m_isInit = false;
    for (auto grp : m_guiGroups)
    {
        grp.second->removeAllWidgets();
    }
    m_guiGroups.clear();

    for (auto& btn : m_buttons)
        btn = nullptr;

    GameState::exit();
}

void MenuState::handleButtons()
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

    if (prevSelected != m_selectedButton)
    {
        m_selectedButton = m_selectedButton < 0 ? m_buttons.size() - 1 : m_selectedButton % m_buttons.size();
        m_buttons[prevSelected]->setFocused(false);
        m_buttons[m_selectedButton]->setFocused(true);
    }
}
} // namespace mmt_gd
