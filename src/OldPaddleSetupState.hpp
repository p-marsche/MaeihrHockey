#pragma once

#include "GameState.hpp"

#include "TGUI/TGUI.hpp"
#include <SFML/Graphics/Text.hpp>

namespace mmt_gd
{
class OldPaddleSetupState final : public GameState
{
public:
    using GameState::GameState;

    void init() override;
    void initGui() override { };

    void update(float delta) override;
    void draw() override;
    void exit() override;

private:
    enum class SetupStep
    {
        Active,
        Passive,
        Finished
    };

    void handleButtons1();
    void handleButtons2();
    void sendEvent();

    tgui::ListBox::Ptr m_player1Active;
    tgui::ListBox::Ptr m_player1Inactive;
    tgui::ListBox::Ptr m_player2Active;
    tgui::ListBox::Ptr m_player2Inactive;

    SetupStep m_player1Progress;
    SetupStep m_player2Progress;
};
} // namespace mmt_gd
