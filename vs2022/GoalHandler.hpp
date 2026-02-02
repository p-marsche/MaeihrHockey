#pragma once

#include "GameObject.hpp"
#include <SFML/System/Vector2.hpp>

namespace mmt_gd
{
	class GoalHandler
	{
	public:
		GoalHandler();
		void setPuck(GameObject::Ptr puck);
		void setSpawn(sf::Vector2f& spawn, int playerIndex);


	private:
		void handleGoal(int spawnIndex);
		GameObject::Ptr m_puck;
		std::array<sf::Vector2f, 2> m_spawns;
	};
}
