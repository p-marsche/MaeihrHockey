#pragma once

#include "GameState.hpp"

#include "TGUI/TGUI.hpp"
#include <SFML/Graphics/Text.hpp>

namespace mmt_gd
{
class PaddleSetupState final : public GameState
{
public:
    using GameState::GameState;

    void init() override;
    void initGui() override { };

    void update(float delta) override;
    void draw() override;
    void exit() override;

private:
    void handleButtons1();
    void handleButtons2();
    void sendEvent();

    sf::View m_view;

    tgui::ListBox::Ptr m_player1Active;
    tgui::ListBox::Ptr m_player1Inactive;
    tgui::ListBox::Ptr m_player2Active;
    tgui::ListBox::Ptr m_player2Inactive;

    bool m_player1ActiveSet;
    bool m_player2ActiveSet;
};
} // namespace mmt_gd
