#pragma once

#include "Config.hpp"
#include "FinalFrontier/dll_export.hpp"

#include <memory>

namespace mmt_gd
{
class GameStateManager;
class Game;

class FINALFRONTIER_API GameState
{
public:
    using Ptr = std::shared_ptr<GameState>;

    GameState(GameStateManager* gameStateManager, Game* game, tgui::Gui* gui) :
    m_gameStateManager(gameStateManager),
    m_game(game),
    m_gui(gui)
    {
    }

    virtual ~GameState()                    = default;
    GameState(const GameState&)             = delete;
    GameState(const GameState&&)            = delete;
    GameState& operator=(const GameState&)  = delete;
    GameState& operator=(const GameState&&) = delete;

    virtual void init()
    {
    }

    virtual void exit()
    {
    }

    virtual void shutdown()
    {
    }

    virtual void update(float delta) = 0;
    virtual void draw()              = 0;
    virtual void initGui()           = 0;

    virtual void disableGui()
    {
        for (auto& group : m_guiGroups)
            group.second->setVisible(false);
    }

    virtual void enableGui()
    {
        for (auto& group : m_guiGroups)
            group.second->setVisible(true);
    }

protected:
    GameStateManager*                                 m_gameStateManager;
    Game*                                             m_game;
    tgui::Gui*                                        m_gui;
    std::unordered_map<std::string, tgui::Group::Ptr> m_guiGroups;
    bool                                              m_isInit = false;
};
} // namespace mmt_gd
