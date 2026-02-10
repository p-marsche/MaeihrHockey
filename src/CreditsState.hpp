#pragma once

#include "GameState.hpp"

#include "TGUI/TGUI.hpp"
#include <SFML/Graphics/Text.hpp>

namespace mmt_gd
{
class CreditsState final : public GameState
{
public:
    using GameState::GameState;

    void init() override;
    void initGui() override { };

    void update(float delta) override;
    void draw() override;
    void exit() override;

private:
    sf::View m_view;

    tgui::Widget::Ptr m_backButton;
};
} // namespace mmt_gd
