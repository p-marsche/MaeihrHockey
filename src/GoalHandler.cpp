#include "stdafx.h"

#include "RigidBodyComponent.hpp"
#include "PhysicsManager.hpp"
#include "GoalHandler.hpp"
#include "GameObjectEvents.hpp"
#include "GoalEvent.hpp"

namespace mmt_gd
{
constexpr float RESET_DELAY = 1.0f;

	GoalHandler::GoalHandler()
		: m_puck(nullptr)
		, m_spawns(std::array<sf::Vector2f, 2>())
		, m_goalNoticed(false)
		, m_respawnSide(0)
		, m_resetTimer(0)
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
                    m_goalNoticed   = true;
                    m_respawnSide   = playerIndex;
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

	void GoalHandler::update(const float deltaTime)
	{
		if (m_goalNoticed)
		{
            if (m_resetTimer < RESET_DELAY)
                m_resetTimer += deltaTime;
            else
            {
                handleGoal(m_respawnSide);
                m_goalNoticed = false;
                m_respawnSide = 0;
                m_resetTimer  = 0;
			}
		}
	}

	void GoalHandler::handleGoal(int spawnIndex)
	{
		sf::Vector2f newPos = m_spawns.at(spawnIndex - 1);
        auto         body   = m_puck->getComponent<RigidBodyComponent>()->getB2Body();
        body->SetTransform(PhysicsManager::s2b(newPos), 0);
        body->SetLinearVelocity(b2Vec2_zero);

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
