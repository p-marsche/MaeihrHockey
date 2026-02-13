//This code was made for the Multimedia Project 2a for the Multimedia Technology class at Fachhochschule Salzburg, by Pascal Veselka & Pascal Marsche
#include "stdafx.h"

#include "ControlsState.hpp"

#include "AssetManager.hpp"
#include "Game.hpp"
#include "GameStateManager.hpp"
#include "InputManager.hpp"

namespace mmt_gd
{
void ControlsState::init()
{
    if (!m_spriteLoaded)
        AssetManager::getInstance().loadTexture("Controls", "Controls.png");

    sf::Texture& tex = AssetManager::getInstance().getTexture("Controls");
    m_controls.setTexture(tex);
}

void ControlsState::update(float deltaTime)
{
    if (InputManager::getInstance().isActionJustPressed("switch"))
        m_gameStateManager->setState("SelectionState");
}

void ControlsState::draw()
{
    sf::RenderWindow& window = m_game->getWindow();
    window.draw(m_controls);
}
}
