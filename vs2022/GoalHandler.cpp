#include "stdafx.h"

#include "RigidBodyComponent.hpp"
#include "PhysicsManager.hpp"
#include "GoalHandler.hpp"

namespace mmt_gd
{
	GoalHandler::GoalHandler()
		: m_puck(nullptr)
		, m_spawns(std::array<sf::Vector2f, 2>())
	{
	}

	void GoalHandler::setPuck(GameObject::Ptr puck)
	{
		m_puck = puck;
	}

	void GoalHandler::setSpawn(sf::Vector2f& spawn, int playerIndex)
	{
		m_spawns.at(playerIndex - 1) = spawn;
	}

	void GoalHandler::handleGoal(int spawnIndex)
	{
		sf::Vector2f newPos = m_spawns.at(spawnIndex - 1);
		m_puck->getComponent<RigidBodyComponent>()->getB2Body()
			->SetTransform(PhysicsManager::s2b(newPos), 0);

		// set acative paddles fixed min distance from middle line?
	}
};
