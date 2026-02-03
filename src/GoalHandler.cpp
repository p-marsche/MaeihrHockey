#include "stdafx.h"

#include "RigidBodyComponent.hpp"
#include "PhysicsManager.hpp"
#include "GoalHandler.hpp"
#include "GameObjectEvents.hpp"
#include "GoalEvent.hpp"

namespace mmt_gd
{
	GoalHandler::GoalHandler()
		: m_puck(nullptr)
		, m_spawns(std::array<sf::Vector2f, 2>())
	{
		// subscribe to creation events
		const EventBus::ListenerId
			creationListenerId = EventBus::getInstance()
			.addListener(GameObjectCreateEvent::Type,
				[this](const IEvent::Ptr& event)
				{
					const auto goCreateEvent = std::static_pointer_cast<GameObjectCreateEvent>(event);
					auto go = goCreateEvent->getData();
					auto id = go->getId();
					if (id == "Puck")
						this->setPuck(go);
					else if (id == "PuckSpawnLeft")
						this->setSpawn((go->getPosition()), 1);
					else if (id == "PuckSpawnRight")
						this->setSpawn((go->getPosition()), 2);
				});
		m_listeners.push_back(creationListenerId);

		const auto goalListenerId = EventBus::getInstance()
			.addListener(GoalEvent::Type,
				[this](const IEvent::Ptr& event)
				{
					const auto goalEvent = std::static_pointer_cast<GoalEvent>(event);
					int playerIndex = goalEvent->getData();
					handleGoal(playerIndex);
				});
		m_listeners.push_back(goalListenerId);
	}

	void GoalHandler::setPuck(GameObject::Ptr puck)
	{
		m_puck = puck;
	}

	void GoalHandler::setSpawn(sf::Vector2f spawn, int playerIndex)
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

	void GoalHandler::shutdown()
	{
		for (auto it = m_listeners.begin(); it != m_listeners.end(); ++it)
		{
			EventBus::getInstance().removeListener(*it);
		}

		m_listeners.clear();
	}
};
