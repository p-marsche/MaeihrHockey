#pragma once

#include "EventBus.hpp"
#include "GameObject.hpp"
#include "PlayerAbilityComponent.hpp"
#include "PlayerMoveComponent.hpp"
#include "SpriteManager.hpp"
#include "SpriteRenderComponent.hpp"

#include <SFML/System/Vector2.hpp>

namespace mmt_gd
{
class Player
{
public:
    Player(const int playerIndex, sf::RenderWindow& window);

    void addPaddle(GameObject::Ptr go);
    void startMatch();
    void update(const float deltaTime);
    void switchPaddle();

    int getplayerIndex()
    {
        return m_playerIndex;
    }

    void shutdown();

private:
    void                                                 createMarkerSprites();
    int                                                  m_playerIndex, m_activeIndex;
    std::vector<GameObject::Ptr>                         m_paddles;
    std::vector<std::shared_ptr<PlayerMoveComponent>>    m_moveComps;
    std::vector<std::shared_ptr<PlayerAbilityComponent>> m_abilityComps;
    std::list<mmt_gd::EventBus::ListenerId>              m_listeners;
    b2Filter                                             m_activeFilterMask, m_passiveFilterMask;
    std::vector<std::shared_ptr<SpriteRenderComponent>>  m_activePaddleMarker;
    sf::RenderWindow&                                    m_window;
};

} //namespace mmt_gd
