//This code was made for the Multimedia Project 2a for the Multimedia Technology class at Fachhochschule Salzburg, by Pascal Veselka & Pascal Marsche
#pragma once

#include "GameState.hpp"

#include <memory>
#include <string>
#include <unordered_map>

namespace mmt_gd
{
class GameStateManager
{
public:
    /**
     * \brief registers a state with a given name
     */

    void registerState(const std::string& name, GameState::Ptr state);

    /**
     * \brief Changes the current GameState to the one with the given name.
     */
    void setState(const std::string& stateName, bool pause = false);

    void update(float deltaTime);
    void draw() const;

    void shutdown() const;

private:
    GameState* findState(const std::string& stateName);
    void       changeState(GameState* stateName, bool pause = false);

    std::unordered_map<std::string, GameState::Ptr> m_states;
    GameState*                                      m_currentState = nullptr;
    GameState*                                      m_futureState  = nullptr;
    bool                                            m_pauseState     = false;
};
} // namespace mmt_gd
