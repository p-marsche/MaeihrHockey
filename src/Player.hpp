#pragma once

#include "GameObject.hpp"
#include "EventBus.hpp"
#include "PlayerMoveComponent.hpp"
#include "PlayerDashComponent.hpp"
#include <SFML/System/Vector2.hpp>

namespace mmt_gd
{
	class Player
	{
	public:
		Player(const int playerIndex);

		void addPaddle(GameObject::Ptr go);
        void startMatch();
        void update(const float deltaTime);
		void switchPaddle();

        int  getplayerIndex()
        {
            return m_playerIndex;
        }

        void shutdown();

	private:
        int										m_playerIndex, m_activeIndex;
		std::vector<GameObject::Ptr>			m_paddles;
        std::vector<std::shared_ptr<PlayerMoveComponent>> m_moveComps;
        std::vector<std::shared_ptr<PlayerDashComponent>> m_dashComps;
		std::list<mmt_gd::EventBus::ListenerId> m_listeners;
        b2Filter                                m_activeFilterMask, m_passiveFilterMask;
	};

} //namespace mmt_gd
