#pragma once

#include "GameObject.hpp"
#include "EventBus.hpp"
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
        void shutdown();

	private:
        int										m_playerIndex, m_activeIndex;
		std::vector<GameObject::Ptr>			m_paddles;
		std::list<mmt_gd::EventBus::ListenerId> m_listeners;
	};

} //namespace mmt_gd
