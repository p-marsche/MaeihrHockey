//This code was made for the Multimedia Project 2a for the Multimedia Technology class at Fachhochschule Salzburg, by Pascal Veselka & Pascal Marsche
#pragma once

#include "GameState.hpp"

#include "TGUI/TGUI.hpp"
#include <SFML/Graphics/Text.hpp>

namespace mmt_gd
{
class PauseState final : public GameState
{
public:
    using GameState::GameState;

    void init() override {};
    void initGui() override {};

    void update(float delta) override {};
    void draw() override {};
    void exit() override {};

private:
    void handleButtons();

    sf::View m_view;

    std::array<tgui::Widget::Ptr, 3> m_buttons;
    int                            m_selectedButton;
};
} // namespace mmt_gd
