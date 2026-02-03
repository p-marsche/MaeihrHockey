#pragma once

#include "GameObject.hpp"
#include <SFML/System/Vector2.hpp>

namespace mmt_gd
{
	class GoalHandler
	{
	public:
		GoalHandler();
        void update(const float deltaTime);
		void setPuck(GameObject::Ptr puck);
		void setSpawn(sf::Vector2f spawn, int playerIndex);
		void shutdown();

	private:
		void handleGoal(int spawnIndex);
		GameObject::Ptr m_puck;
		std::array<sf::Vector2f, 2> m_spawns;
		std::list<mmt_gd::EventBus::ListenerId> m_listeners;
        bool                                    m_goalNoticed;
        int                                     m_respawnSide;
        float                                   m_resetTimer;
	};
}
