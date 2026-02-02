#include "stdafx.h"
#include "GameObjectFactory.hpp"

#include "AssetManager.hpp"
#include "ColliderComponent.hpp"
#include "EventBus.hpp"
#include "GameObjectEvents.hpp"
#include "ObjectFactory.hpp"
#include "PhysicsManager.hpp"
#include "PlayerMoveComponent.hpp"
#include "RigidBodyComponent.hpp"
#include "SpriteRenderComponent.hpp"

namespace mmt_gd
{
	GameObject::Ptr GameObjectFactory::createPuck(sf::RenderWindow& window
													, tson::Object& obj)
	{
		auto puck = GameObject::create("Puck");
		sf::Vector2f pos = ObjectFactory::getPosition(obj);
		puck->setPosition(pos);
		
		std::string textureKey = ObjectFactory::getTexture(obj);
		sf::Texture tex = AssetManager::getInstance().getTexture(textureKey);
		auto spriteComp = puck->addComponent<SpriteRenderComponent>(
			*puck, window, tex, "GameObjects",
			sf::IntRect(0, 0, 0, 0));

		auto rigidBody = puck->addComponent<RigidBodyComponent>(*puck, b2_dynamicBody);
		rigidBody->getB2Body()->SetFixedRotation(true);
		rigidBody->getB2Body()->SetLinearDamping(0.3f);

		b2PolygonShape shape;
		const auto size = PhysicsManager::s2b(ObjectFactory::getSize(obj));
		const auto shapeType = ObjectFactory::getShape(obj);
		if (shapeType == "Circle")
		{
			b2CircleShape shape;
			shape.m_p.SetZero();
			shape.m_radius = size.x / 2;
		}
		else
		{
			b2PolygonShape shape;
			shape.SetAsBox(size.x / 2, size.y / 2, b2Vec2{ size.x / 2, size.y / 2 }, 0);
		}
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		fixtureDef.density = ObjectFactory::getDensity(obj);

		b2Filter filter;
		filter.categoryBits = CollisionLayers::OBJECTS;
		filter.maskBits = CollisionLayers::GOAL_SENSOR || CollisionLayers::OBJECTS || CollisionLayers::WALL;
		fixtureDef.filter = filter;

		auto collider = puck->addComponent<ColliderComponent>(*puck, *rigidBody, fixtureDef);

		// add onCollision func later if we actually need it
		/*collider->registerOnCollisionFunction([](ColliderComponent& self, ColliderComponent& other) {

		});*/

		if (!puck->init())
		{
			sf::err() << "Could not initialize player ship\n";
		}

		EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(puck));

		return puck;
	}

	GameObject::Ptr GameObjectFactory::createPaddle(sf::RenderWindow& window
													, tson::Object& obj
													, int paddleID)
	{
		int playerIndex = ObjectFactory::getPlayerIndex(obj);
		auto paddle = GameObject::create("Paddle" + std::to_string(paddleID));
		sf::Vector2f pos = ObjectFactory::getPosition(obj);
		paddle->setPosition(pos);

		std::string textureKey = ObjectFactory::getTexture(obj);
		sf::Texture tex = AssetManager::getInstance().getTexture(textureKey);
		auto spriteComp = paddle->addComponent<SpriteRenderComponent>(
			*paddle, window, tex, "GameObjects",
			sf::IntRect(0, 0, 0, 0));

		auto rigidBody = paddle->addComponent<RigidBodyComponent>(*paddle, b2_dynamicBody);
		rigidBody->getB2Body()->SetFixedRotation(true);
		rigidBody->getB2Body()->SetLinearDamping(0.3f);

		b2PolygonShape shape;
		const auto size = PhysicsManager::s2b(ObjectFactory::getSize(obj));
		const auto shapeType = ObjectFactory::getShape(obj);
		if (shapeType == "Circle")
		{
			b2CircleShape shape;
			shape.m_p.SetZero();
			shape.m_radius = size.x / 2;
		}
		else
		{
			b2PolygonShape shape;
			shape.SetAsBox(size.x / 2, size.y / 2, b2Vec2{ size.x / 2, size.y / 2 }, 0);
		}
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		fixtureDef.density = ObjectFactory::getDensity(obj);

		b2Filter filter;
		filter.categoryBits = CollisionLayers::OBJECTS;
		filter.maskBits = CollisionLayers::GOAL_SENSOR || CollisionLayers::OBJECTS || CollisionLayers::WALL;
		fixtureDef.filter = filter;

		auto collider = paddle->addComponent<ColliderComponent>(*paddle, *rigidBody, fixtureDef);

		// add onCollision func later if we actually need it
		/*collider->registerOnCollisionFunction([](ColliderComponent& self, ColliderComponent& other) {

		});*/

		if (!paddle->init())
		{
			sf::err() << "Could not initialize player ship\n";
		}

		EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(paddle));

		return paddle;
	}

	GameObject::Ptr GameObjectFactory::createWall(sf::RenderWindow& window
													, tson::Object& obj)
	{

	}

	GameObject::Ptr GameObjectFactory::createNeutralzone(sf::RenderWindow& window
															, tson::Object& obj)
	{

	}

	GameObject::Ptr GameObjectFactory::createPenaltyarea(sf::RenderWindow& window
															, tson::Object& obj)
	{

	}

	GameObject::Ptr GameObjectFactory::createGoalsensor(sf::RenderWindow& window
														, tson::Object& obj)
	{

	}

	GameObject::Ptr GameObjectFactory::createGoalbarrier(sf::RenderWindow& window
															, tson::Object& obj)
	{

	}
}