#include "stdafx.h"

#include "GameStateManager.hpp"
#include "PlayerConfig.hpp"
#include "MainState.hpp"
#include "EventBus.hpp"
#include "PlayerConfigEvent.hpp"
//#include "PreviewState.hpp"

#include "Debug.hpp"
#include "GameState.hpp"

#include <utility>

namespace mmt_gd
{
void GameStateManager::registerState(const std::string& name, GameState::Ptr state)
{
    m_states[name] = std::move(state);
}

void GameStateManager::setState(const std::string& stateName, bool pause)
{
    GameState* state = findState(stateName);
    ffAssertMsg(state != nullptr, "State could not be found")

    /*if (stateName == "MainState")
    {
        /*auto prev = static_cast<PreviewState*>(m_currentState);
        auto main = static_cast<MainState*>(state);
        main->addConfig(prev->getConfig());*/


        
        ////Config Test Stuff
        PlayerConfig config1{};
        PlayerConfig config2{};
        config1.m_playerIndex = 0;
        config2.m_playerIndex = 1;

        for (int i = 0; i < 3; ++i)
        {
            config1.m_config.at(i).m_ability = PaddleAbility::DASH;
            config1.m_config.at(i).m_passive = PaddlePassive::HEAVY;
            config1.m_config.at(i).m_paddleIndex = i;
        }

        for (int i = 0; i < 3; ++i)
        {
            config2.m_config.at(i).m_ability     = PaddleAbility::ENLARGE;
            config2.m_config.at(i).m_passive     = PaddlePassive::BOUNCY;
            config2.m_config.at(i).m_paddleIndex = i;
        }

        //config2.m_config.at(2).m_ability = PaddleAbility::ENLARGE;
        //config2.m_config.at(2).m_passive = PaddlePassive::BOUNCY;
        //config2.m_config.at(2).m_paddleIndex = 3;

        EventBus::getInstance().fireEvent(std::make_shared<PlayerConfigFinishEvent>(config1));
        EventBus::getInstance().fireEvent(std::make_shared<PlayerConfigFinishEvent>(config2));
    //}

    m_futureState = state;
    m_pauseState      = pause;
}

void GameStateManager::changeState(GameState* stateName, bool pause)
{
    if (stateName != m_currentState)
    {
        if (m_currentState != nullptr)
        {
            m_currentState->disableGui();
            m_currentState->exit();
        }

        m_currentState = stateName;

        if (m_currentState != nullptr)
        {
            m_currentState->init();
        }
    }
}

void GameStateManager::update(float deltaTime)
{
    if (m_futureState != nullptr)
    {
        changeState(m_futureState, m_pauseState);
        m_futureState = nullptr;
    }

    if (m_currentState != nullptr)
    {
        m_currentState->update(deltaTime);
    }
}

void GameStateManager::draw() const
{
    if (m_currentState != nullptr)
    {
        m_currentState->draw();
    }
}

void GameStateManager::shutdown() const
{
    if (m_currentState != nullptr)
    {
        m_currentState->exit();
    }
}

GameState* GameStateManager::findState(const std::string& stateName)
{
    const auto state = m_states.find(stateName);
    if (state != m_states.end())
    {
        return state->second.get();
    }
    return nullptr;
}
} // namespace mmt_gd
