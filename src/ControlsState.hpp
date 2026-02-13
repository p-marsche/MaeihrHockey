#pragma once

#include "GameState.hpp"
#include <SFML/Graphics/Sprite.hpp>

namespace mmt_gd
{
class ControlsState final : public GameState
{
public:
    using GameState::GameState;

    void init() override;
    void initGui() override {};

    void update(float delta) override;
    void draw() override;

private:
    sf::Sprite m_controls;
    bool       m_spriteLoaded = false;
};
} // namespace mmt_gd
