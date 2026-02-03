#include "stdafx.h"

#include "GameObjectFactory.hpp"

#include "AssetManager.hpp"
#include "ColliderComponent.hpp"
#include "EventBus.hpp"
#include "GameObjectEvents.hpp"
#include "PhysicsManager.hpp"
#include "PlayerMoveComponent.hpp"
#include "RigidBodyComponent.hpp"
#include "SpriteRenderComponent.hpp"
#include "TsonPropertyReader.hpp"

namespace mmt_gd
{
void GameObjectFactory::createGameObject(sf::RenderWindow& window, tson::Object& obj)
{
    std::string     objType = obj.getType();
    GameObject::Ptr ptr     = nullptr;
    if (objType == "Puck")
        ptr = createPuck(window, obj);
    else if (objType == "Paddle")
        ptr = createPaddle(window, obj);
    else if (objType == "Wall")
        ptr = createWall(window, obj);
    else if (objType == "InvisibleWall")
        ptr = createNeutralzone(window, obj);
    else if (objType == "PenaltyArea")
        ptr = createPenaltyarea(window, obj);
    else if (objType == "Goal")
        ptr = createGoalsensor(window, obj);
    else if (objType == "PuckSpawn")
        ptr = createPuckSpawn(window, obj);
    else
    {
        std::cerr << "ERROR: GameObjectFactory::createGameObject: Unknown object type \"" << objType << "\"";
        return;
    }

    if (ptr == nullptr)
        std::cerr << "ERROR: GameObjectFactory::createGameObject: Failed to create object of type \"" << objType << "\"";
}

GameObject::Ptr GameObjectFactory::createPuck(sf::RenderWindow& window, tson::Object& obj)
{
    auto puck = createObject(obj);

    addSpriteRenderer(obj, *puck, window);

    auto rigidBody = puck->addComponent<RigidBodyComponent>(*puck, b2_dynamicBody);
    rigidBody->getB2Body()->SetFixedRotation(true);
    rigidBody->getB2Body()->SetLinearDamping(0.1f);

    b2FixtureDef fixtureDef = createFixtureDef(obj);
    b2Filter     filter;
    filter.categoryBits = CollisionLayers::OBJECTS;
    filter.maskBits     = CollisionLayers::GOAL_SENSOR | CollisionLayers::OBJECTS | CollisionLayers::WALL;
    fixtureDef.filter   = filter;

    auto collider = puck->addComponent<ColliderComponent>(*puck, *rigidBody, fixtureDef);

    // add onCollision func later if we actually need it
    /*collider->registerOnCollisionFunction([](ColliderComponent& self, ColliderComponent& other) {

		});*/

    if (!puck->init())
    {
        sf::err() << "Could not initialize puck\n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(puck));

    return puck;
}

GameObject::Ptr GameObjectFactory::createPaddle(sf::RenderWindow& window, tson::Object& obj)
{
    //int  playerIndex = TsonPropertyReader::getPlayerIndex(obj);
    auto paddle      = createObject(obj);

    addSpriteRenderer(obj, *paddle, window);

    auto rigidBody = paddle->addComponent<RigidBodyComponent>(*paddle, b2_dynamicBody);
    rigidBody->getB2Body()->SetFixedRotation(true);
    rigidBody->getB2Body()->SetLinearDamping(0.3f);

    b2FixtureDef fixtureDef = createFixtureDef(obj);
    b2Filter     filter;
    filter.categoryBits = CollisionLayers::OBJECTS;
    filter.maskBits     = CollisionLayers::FAKE_WALL | CollisionLayers::OBJECTS | CollisionLayers::WALL |
                      CollisionLayers::PENALTY;
    fixtureDef.filter = filter;

    auto collider = paddle->addComponent<ColliderComponent>(*paddle, *rigidBody, fixtureDef);

    // add onCollision func later if we actually need it
    /*collider->registerOnCollisionFunction([](ColliderComponent& self, ColliderComponent& other) {

		});*/

    if (!paddle->init())
    {
        sf::err() << "Could not initialize paddle \n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(paddle));

    return paddle;
}

GameObject::Ptr GameObjectFactory::createWall(sf::RenderWindow& window, tson::Object& obj)
{
    auto wall = createObject(obj);

    /*std::string textureKey = TsonPropertyReader::getTexture(obj);
		sf::Texture tex = AssetManager::getInstance().getTexture(textureKey);
		auto spriteComp = wall->addComponent<SpriteRenderComponent>(
			*paddle, window, tex, "GameObjects",
			sf::IntRect(0, 0, 0, 0));*/

    auto rigidBody = wall->addComponent<RigidBodyComponent>(*wall, b2_staticBody);

    b2FixtureDef fixtureDef = createFixtureDef(obj);
    b2Filter     filter;
    filter.categoryBits = CollisionLayers::WALL;
    filter.maskBits     = CollisionLayers::OBJECTS;
    fixtureDef.filter   = filter;

    auto collider = wall->addComponent<ColliderComponent>(*wall, *rigidBody, fixtureDef);

    // add onCollision func later if we actually need it
    /*collider->registerOnCollisionFunction([](ColliderComponent& self, ColliderComponent& other) {

		});*/

    if (!wall->init())
    {
        sf::err() << "Could not initialize wall \n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(wall));

    auto fixPos = PhysicsManager::b2s(wall->getComponent<ColliderComponent>()->getBody().getB2Body()->GetPosition());

    /*std::cout << obj.getName() << std::endl;
    std::cout << obj.getPosition().x << "  :  " << obj.getPosition().y << std::endl;
    std::cout << "fixture: " <<  fixPos.x << "  :  " << fixPos.y << std::endl;
    std::cout << obj.getSize().x << "  :  " << obj.getSize().y << std::endl;
    std::cout << std::endl;*/

    return wall;
}

GameObject::Ptr GameObjectFactory::createNeutralzone(sf::RenderWindow& window, tson::Object& obj)
{
    auto neutral = createObject(obj);

    /*std::string textureKey = TsonPropertyReader::getTexture(obj);
		sf::Texture tex = AssetManager::getInstance().getTexture(textureKey);
		auto spriteComp = wall->addComponent<SpriteRenderComponent>(
			*paddle, window, tex, "GameObjects",
			sf::IntRect(0, 0, 0, 0));*/

    auto rigidBody = neutral->addComponent<RigidBodyComponent>(*neutral, b2_staticBody);

    b2FixtureDef fixtureDef = createFixtureDef(obj);
    b2Filter     filter;
    filter.categoryBits = CollisionLayers::FAKE_WALL;
    filter.maskBits     = CollisionLayers::OBJECTS;
    fixtureDef.filter   = filter;

    auto collider = neutral->addComponent<ColliderComponent>(*neutral, *rigidBody, fixtureDef);

    // add onCollision func later if we actually need it
    /*collider->registerOnCollisionFunction([](ColliderComponent& self, ColliderComponent& other) {

		});*/

    if (!neutral->init())
    {
        sf::err() << "Could not initialize wall \n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(neutral));

    return neutral;
}

GameObject::Ptr GameObjectFactory::createPenaltyarea(sf::RenderWindow& window, tson::Object& obj)
{
    auto penalty = createObject(obj);

    /*std::string textureKey = TsonPropertyReader::getTexture(obj);
		sf::Texture tex = AssetManager::getInstance().getTexture(textureKey);
		auto spriteComp = wall->addComponent<SpriteRenderComponent>(
			*paddle, window, tex, "GameObjects",
			sf::IntRect(0, 0, 0, 0));*/

    auto rigidBody = penalty->addComponent<RigidBodyComponent>(*penalty, b2_staticBody);

    b2FixtureDef fixtureDef = createFixtureDef(obj);
    b2Filter     filter;
    filter.categoryBits = CollisionLayers::PENALTY;
    filter.maskBits     = CollisionLayers::OBJECTS;
    fixtureDef.filter   = filter;

    auto collider = penalty->addComponent<ColliderComponent>(*penalty, *rigidBody, fixtureDef);

    // add onCollision func later if we actually need it
    /*collider->registerOnCollisionFunction([](ColliderComponent& self, ColliderComponent& other) {

		});*/

    if (!penalty->init())
    {
        sf::err() << "Could not initialize wall \n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(penalty));

    return penalty;
}

GameObject::Ptr GameObjectFactory::createGoalsensor(sf::RenderWindow& window, tson::Object& obj)
{
    auto goal = createObject(obj);

    /*std::string textureKey = TsonPropertyReader::getTexture(obj);
		sf::Texture tex = AssetManager::getInstance().getTexture(textureKey);
		auto spriteComp = wall->addComponent<SpriteRenderComponent>(
			*paddle, window, tex, "GameObjects",
			sf::IntRect(0, 0, 0, 0));*/

    auto rigidBody = goal->addComponent<RigidBodyComponent>(*goal, b2_staticBody);

    b2FixtureDef fixtureDef = createFixtureDef(obj);
    b2Filter     filter;
    filter.categoryBits = CollisionLayers::GOAL_SENSOR;
    filter.maskBits     = CollisionLayers::OBJECTS;
    fixtureDef.filter   = filter;

    auto collider = goal->addComponent<ColliderComponent>(*goal, *rigidBody, fixtureDef);

    // add onCollision func later if we actually need it
    /*collider->registerOnCollisionFunction([](ColliderComponent& self, ColliderComponent& other) {

		});*/

    if (!goal->init())
    {
        sf::err() << "Could not initialize wall \n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(goal));

    return goal;
}

GameObject::Ptr GameObjectFactory::createGoalbarrier(sf::RenderWindow& window, tson::Object& obj)
{
    auto gb = createObject(obj);

    /*std::string textureKey = TsonPropertyReader::getTexture(obj);
		sf::Texture tex = AssetManager::getInstance().getTexture(textureKey);
		auto spriteComp = wall->addComponent<SpriteRenderComponent>(
			*paddle, window, tex, "GameObjects",
			sf::IntRect(0, 0, 0, 0));*/

    auto rigidBody = gb->addComponent<RigidBodyComponent>(*gb, b2_staticBody);

    b2FixtureDef fixtureDef = createFixtureDef(obj);
    b2Filter     filter;
    filter.categoryBits = CollisionLayers::WALL;
    filter.maskBits     = CollisionLayers::OBJECTS;
    fixtureDef.filter   = filter;

    auto collider = gb->addComponent<ColliderComponent>(*gb, *rigidBody, fixtureDef);

    // add onCollision func later if we actually need it
    /*collider->registerOnCollisionFunction([](ColliderComponent& self, ColliderComponent& other) {

		});*/

    if (!gb->init())
    {
        sf::err() << "Could not initialize wall \n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gb));

    return gb;
}

GameObject::Ptr GameObjectFactory::createPuckSpawn(sf::RenderWindow& window, tson::Object& obj)
{
    auto spawn = createObject(obj);

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(spawn));

    return spawn;
}

GameObject::Ptr GameObjectFactory::createObject(tson::Object& obj)
{
    auto         go  = GameObject::create(obj.getName());
    sf::Vector2f pos = t2s(obj.getPosition());
    sf::Vector2f size = t2s(obj.getSize());
    auto         newPos = sf::Vector2f(pos.x + (size.x / 2), pos.y + (size.y / 2));
    go->setPosition(newPos);
    return go;
}

void GameObjectFactory::addSpriteRenderer(tson::Object& obj, GameObject& go, sf::RenderWindow& window)
{
    std::string textureKey = TsonPropertyReader::getTexture(obj);
    AssetManager::getInstance().loadTexture(textureKey, Config::imagesPath + textureKey);
    auto spriteComp = go.addComponent<SpriteRenderComponent>(go,
                                                             window,
                                                             AssetManager::getInstance().getTexture(textureKey),
                                                             "GameObjects",
                                                             sf::IntRect(0, 0, 0, 0));
    auto  textureSize = AssetManager::getInstance().getTexture(textureKey).getSize();
    float scale       = t2s(obj.getSize()).x / textureSize.x;
    spriteComp->getSprite().setOrigin(textureSize.x / 2, textureSize.y / 2);
    spriteComp->setScale(scale, scale);
}

b2FixtureDef GameObjectFactory::createFixtureDef(tson::Object& obj)
{
    const auto   size      = PhysicsManager::t2b(obj.getSize());
    const auto   shapeType = TsonPropertyReader::getShape(obj);
    b2FixtureDef fixtureDef;
    if (shapeType == "Circle")
    {
        b2CircleShape* shape = new b2CircleShape();
        shape->m_p.SetZero();
        shape->m_radius   = size.x / 2;
        fixtureDef.shape = shape;
    }
    else
    {
        b2PolygonShape* shape = new b2PolygonShape();
        shape->SetAsBox(size.x / 2, size.y / 2, b2Vec2{0, 0}, 0);
        fixtureDef.shape = shape;
    }
    fixtureDef.density  = TsonPropertyReader::getDensity(obj);
    fixtureDef.isSensor = TsonPropertyReader::isSensor(obj);

    return fixtureDef;
}

sf::Vector2f GameObjectFactory::t2s(tson::Vector2i vec)
{
    return sf::Vector2f(vec.x, vec.y);
}
} // namespace mmt_gd