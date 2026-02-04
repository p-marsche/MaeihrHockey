#include "stdafx.h"

#include "Player.hpp"
#include "PlayerMoveComponent.hpp"
#include "RigidBodyComponent.hpp"
#include "EventBus.hpp"
#include "GameObjectEvents.hpp"
#include "InputManager.hpp"
#include <sstream>

namespace mmt_gd
{
	Player::Player(const int playerIndex)
		: m_listeners()
		, m_paddles()
		, m_moveComps()
		, m_playerIndex(playerIndex)
	{
		const EventBus::ListenerId
			creationListenerId = EventBus::getInstance()
			.addListener(GameObjectCreateEvent::Type,
				[this, playerIndex](const IEvent::Ptr& event)
				{
					const auto goCreateEvent = std::static_pointer_cast<GameObjectCreateEvent>(event);
					auto go = goCreateEvent->getData();
					std::stringstream fullname(go->getId());
                    std::string targetname = "Player" + std::to_string(playerIndex+1);
					std::string segment;
					std::getline(fullname, segment, '-');
					if (segment == targetname)
						addPaddle(go);
				});
		m_listeners.push_back(creationListenerId);
	}

	void Player::startMatch()
    {
        std::array<GameObject::Ptr, 3> temp;

		for (auto p : m_paddles)
        {
            int length = p->getId().length();
            std::string idx    = "";
            idx.push_back(p->getId().at(length - 1));
            int index = stoi(idx);
            temp[index-1] = p;
        }
        m_paddles.clear();

		for (int i = 0; i < 3; ++i)
        {
            m_paddles.push_back(temp[i]);
            auto rb = m_paddles[i]->getComponent<RigidBodyComponent>();
            m_moveComps.push_back(std::make_shared<PlayerMoveComponent>(*m_paddles[i], *rb, m_playerIndex));
        }

        m_paddles[1]->addComponent<PlayerMoveComponent>(m_moveComps[1]);
        m_activeIndex = 1;
	}

	void Player::update(const float deltaTime)
	{
        if (InputManager::getInstance().isKeyPressed("switch", m_playerIndex))
            switchPaddle();
	}

	void Player::addPaddle(GameObject::Ptr go)
	{
		std::stringstream fullname(go->getId());
		std::string segment;
		std::vector<std::string> parts;

		while (std::getline(fullname, segment, '-'))
		{
			parts.push_back(segment);
		}

		int namelength = parts.at(0).length();
        std::string playerIdx = "";
        playerIdx.push_back(parts.at(0).at(namelength - 1));
		int playerIndex = std::stoi(playerIdx) - 1;
		int paddleIndex = std::stoi(parts.at(1));

		m_paddles.push_back(go);
	}

	void Player::switchPaddle()
	{
        auto go1 = m_paddles[m_activeIndex];
        go1->removeComponent(m_moveComps[m_activeIndex]);

        if (m_activeIndex == m_paddles.size() - 1)
            m_activeIndex = 0;
        else
            m_activeIndex++;

		auto go2 = m_paddles[m_activeIndex];
        go2->addComponent<PlayerMoveComponent>(m_moveComps[m_activeIndex]);
	}

	void Player::shutdown()
	{
        for (auto it = m_listeners.begin(); it != m_listeners.end(); ++it)
        {
            EventBus::getInstance().removeListener(*it);
        }

        m_listeners.clear();
	}
}
