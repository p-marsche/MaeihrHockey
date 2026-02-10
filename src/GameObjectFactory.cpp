#include "stdafx.h"

#include "GameObjectFactory.hpp"

#include "AssetManager.hpp"
#include "AudioComponent.hpp"
#include "ColliderComponent.hpp"
#include "EventBus.hpp"
#include "GoalEvent.hpp"
#include "GameObjectEvents.hpp"
#include "PhysicsManager.hpp"
#include "PlayerMoveComponent.hpp"
#include "RigidBodyComponent.hpp"
#include "SpriteRenderComponent.hpp"
#include "TsonPropertyReader.hpp"

namespace mmt_gd
{
using Parser = TsonPropertyReader;

float constexpr WALL_KNOCKBACK     = 0.3f;
float constexpr PADDLE_DAMPING     = 0.2f;
float constexpr PADDLE_RESTITUTION = 0.7f;
float constexpr PUCK_DAMPING       = 0.05f;
float constexpr PUCK_RESTITUTION   = 1.f;

void GameObjectFactory::createGameObject(sf::RenderWindow& window, ObjectFormat& obj)
{
    std::string     objType = Parser::getType(obj);
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
    else if (objType == "ExtraWall")
        ptr = createExtraWall(window, obj);
    else
    {
        std::cerr << "ERROR: GameObjectFactory::createGameObject: Unknown object type \"" << objType << "\"";
        return;
    }

    if (ptr == nullptr)
        std::cerr << "ERROR: GameObjectFactory::createGameObject: Failed to create object of type \"" << objType << "\"";
}

GameObject::Ptr GameObjectFactory::createPuck(sf::RenderWindow& window, ObjectFormat& obj)
{
    auto puck = createObject(obj);

    addSpriteRenderer(obj, *puck, window);

    auto rigidBody = puck->addComponent<RigidBodyComponent>(*puck, b2_dynamicBody);
    auto body      = rigidBody->getB2Body();
    body->SetFixedRotation(true);
    body->SetLinearDamping(PUCK_DAMPING);
    body->SetBullet(true);

    b2FixtureDef fixtureDef = createFixtureDef(obj);
    b2Filter     filter;
    filter.categoryBits = CollisionLayers::OBJECTS;
    filter.maskBits     = CollisionLayers::GOAL_SENSOR | CollisionLayers::OBJECTS | 
        CollisionLayers::WALL | CollisionLayers::PUCK_WALL;
    fixtureDef.filter   = filter;
    fixtureDef.restitution = PUCK_RESTITUTION;

    auto collider = puck->addComponent<ColliderComponent>(*puck, *rigidBody, fixtureDef);
    sf::SoundBuffer& buffer   = AssetManager::getInstance().getSoundBuffer("test");
    auto             audio    = puck->addComponent<AudioComponent>(*puck, buffer);

    collider->registerOnCollisionFunction(
        [audio](ColliderComponent& self, ColliderComponent& other)
        { 
            audio->playSound();
        });

    float knockback = 50.f;

    collider->registerOnCollisionFunction(
        [knockback](ColliderComponent& self, ColliderComponent& other) 
        { 
            if (other.getGameObject().getId() == "TopWall")
                self.getBody().addVelocity(sf::Vector2f(0.f, knockback));
            else if (other.getGameObject().getId() == "BottomWall")
                self.getBody().addVelocity(sf::Vector2f(0.f, -1 * knockback));
        });

    if (!puck->init())
    {
        sf::err() << "Could not initialize puck\n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(puck));

    return puck;
}

GameObject::Ptr GameObjectFactory::createPaddle(sf::RenderWindow& window, ObjectFormat& obj)
{
    int  playerIndex = Parser::getPlayerIndex(obj);
    auto paddle      = createObject(obj);

    addSpriteRenderer(obj, *paddle, window);

    auto rigidBody = paddle->addComponent<RigidBodyComponent>(*paddle, b2_dynamicBody);
    rigidBody->getB2Body()->SetFixedRotation(true);
    rigidBody->getB2Body()->SetLinearDamping(PADDLE_DAMPING);

    b2FixtureDef fixtureDef = createFixtureDef(obj);
    b2Filter     filter;
    filter.categoryBits = CollisionLayers::OBJECTS;

    if (playerIndex == 0)
        filter.categoryBits = filter.categoryBits | CollisionLayers::PLAYER_1;
    else if (playerIndex == 1)
        filter.categoryBits = filter.categoryBits | CollisionLayers::PLAYER_2;

    filter.maskBits     = CollisionLayers::FAKE_WALL | CollisionLayers::OBJECTS | 
                        CollisionLayers::WALL | CollisionLayers::PENALTY;
    fixtureDef.filter = filter;
    fixtureDef.restitution = PADDLE_RESTITUTION;

    auto collider = paddle->addComponent<ColliderComponent>(*paddle, *rigidBody, fixtureDef);

    if (!paddle->init())
    {
        sf::err() << "Could not initialize paddle \n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(paddle));

    return paddle;
}

GameObject::Ptr GameObjectFactory::createWall(sf::RenderWindow& window, ObjectFormat& obj)
{
    auto wall = createObject(obj);

    auto rigidBody = wall->addComponent<RigidBodyComponent>(*wall, b2_staticBody);

    b2FixtureDef fixtureDef = createFixtureDef(obj);
    b2Filter     filter;
    filter.categoryBits = CollisionLayers::WALL;
    filter.maskBits     = CollisionLayers::OBJECTS;
    fixtureDef.filter   = filter;

    auto collider = wall->addComponent<ColliderComponent>(*wall, *rigidBody, fixtureDef);

    if (!wall->init())
    {
        sf::err() << "Could not initialize wall \n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(wall));

    float dir = 0.f;
    if (TsonPropertyReader::getName(obj) == "TopWall") 
        dir = (-1.0 * WALL_KNOCKBACK);
    else if (TsonPropertyReader::getName(obj) == "BottomWall") 
        dir = WALL_KNOCKBACK;

    collider->registerOnCollisionFunction(
        [dir](ColliderComponent& self, ColliderComponent& other)
        { 
            if (!(other.getGameObject().getId() == "Puck"))
                return;
            auto vel = other.getBody().getB2Body()->GetLinearVelocity();
            other.getBody().getB2Body()->SetLinearVelocity(b2Vec2(vel.x, vel.y+dir)); 
        });

    return wall;
}

GameObject::Ptr GameObjectFactory::createNeutralzone(sf::RenderWindow& window, ObjectFormat& obj)
{
    auto neutral = createObject(obj);

    auto rigidBody = neutral->addComponent<RigidBodyComponent>(*neutral, b2_staticBody);

    b2FixtureDef fixtureDef = createFixtureDef(obj);
    b2Filter     filter;
    filter.categoryBits = CollisionLayers::FAKE_WALL;
    switch (Parser::getPlayerIndex(obj))
    {
        case 0:
            filter.maskBits = CollisionLayers::PLAYER_1;
            break;
        case 1:
            filter.maskBits = CollisionLayers::PLAYER_2;
            break;
        default:
            filter.maskBits = CollisionLayers::OBJECTS;
            break;
    }
    fixtureDef.filter   = filter;

    auto collider = neutral->addComponent<ColliderComponent>(*neutral, *rigidBody, fixtureDef);

    if (!neutral->init())
    {
        sf::err() << "Could not initialize wall \n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(neutral));

    return neutral;
}

GameObject::Ptr GameObjectFactory::createExtraWall(sf::RenderWindow& window, ObjectFormat& obj)
{
    auto neutral = createObject(obj);

    auto rigidBody = neutral->addComponent<RigidBodyComponent>(*neutral, b2_staticBody);

    b2FixtureDef fixtureDef = createFixtureDef(obj);
    b2Filter     filter;
    filter.categoryBits = CollisionLayers::PUCK_WALL;
    filter.maskBits     = CollisionLayers::OBJECTS;
    fixtureDef.filter   = filter;

    fixtureDef.isSensor = true;

    auto collider = neutral->addComponent<ColliderComponent>(*neutral, *rigidBody, fixtureDef);

    float dir = (TsonPropertyReader::getWallSide(obj) == "Top") ? WALL_KNOCKBACK : -WALL_KNOCKBACK; 

    collider->registerOnCollisionFunction([dir](ColliderComponent& self, ColliderComponent& other) 
        { 
            other.getBody().getB2Body()->ApplyLinearImpulseToCenter(b2Vec2(0.f, dir), true);
		});

    if (!neutral->init())
    {
        sf::err() << "Could not initialize wall \n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(neutral));

    return neutral;
}

GameObject::Ptr GameObjectFactory::createPenaltyarea(sf::RenderWindow& window, ObjectFormat& obj)
{
    auto penalty = createObject(obj);

    auto rigidBody = penalty->addComponent<RigidBodyComponent>(*penalty, b2_staticBody);

    b2FixtureDef fixtureDef = createFixtureDef(obj);
    b2Filter     filter;
    filter.categoryBits = CollisionLayers::PENALTY;
    filter.maskBits     = CollisionLayers::OBJECTS;
    fixtureDef.filter   = filter;

    auto collider = penalty->addComponent<ColliderComponent>(*penalty, *rigidBody, fixtureDef);

    if (!penalty->init())
    {
        sf::err() << "Could not initialize wall \n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(penalty));

    return penalty;
}

GameObject::Ptr GameObjectFactory::createGoalsensor(sf::RenderWindow& window, ObjectFormat& obj)
{
    auto goal = createObject(obj);

    auto rigidBody = goal->addComponent<RigidBodyComponent>(*goal, b2_staticBody);

    b2FixtureDef fixtureDef = createFixtureDef(obj);
    b2Filter     filter;
    filter.categoryBits = CollisionLayers::GOAL_SENSOR;
    filter.maskBits     = CollisionLayers::OBJECTS;
    fixtureDef.filter   = filter;
    fixtureDef.isSensor = true;

    auto collider = goal->addComponent<ColliderComponent>(*goal, *rigidBody, fixtureDef);
    int  sideID;
    if (goal->getId() == "LeftGoal")
        sideID = 1;
    else if (goal->getId() == "RightGoal")
        sideID = 2;

    collider->registerOnCollisionFunction([sideID](ColliderComponent& self, ColliderComponent& other) {
            auto otherId = other.getGameObject().getId();
            if (otherId == "Puck")
                EventBus::getInstance().fireEvent(std::make_shared<GoalEvent>(sideID));
		});

    if (!goal->init())
    {
        sf::err() << "Could not initialize wall \n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(goal));

    return goal;
}

GameObject::Ptr GameObjectFactory::createGoalbarrier(sf::RenderWindow& window, ObjectFormat& obj)
{
    auto gb = createObject(obj);

    auto rigidBody = gb->addComponent<RigidBodyComponent>(*gb, b2_staticBody);

    b2FixtureDef fixtureDef = createFixtureDef(obj);
    b2Filter     filter;
    filter.categoryBits = CollisionLayers::WALL;
    filter.maskBits     = CollisionLayers::OBJECTS;
    fixtureDef.filter   = filter;

    auto collider = gb->addComponent<ColliderComponent>(*gb, *rigidBody, fixtureDef);

    if (!gb->init())
    {
        sf::err() << "Could not initialize wall \n";
    }

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gb));

    return gb;
}

GameObject::Ptr GameObjectFactory::createPuckSpawn(sf::RenderWindow& window, ObjectFormat& obj)
{
    auto spawn = createObject(obj);

    EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(spawn));

    return spawn;
}

GameObject::Ptr GameObjectFactory::createObject(ObjectFormat& obj)
{
    auto         go  = GameObject::create(Parser::getName(obj));
    sf::Vector2f pos = Parser::getPosition(obj);
    sf::Vector2f size = Parser::getSize(obj);
    auto         newPos = sf::Vector2f(pos.x + (size.x / 2), pos.y + (size.y / 2));
    go->setPosition(newPos);
    return go;
}

void GameObjectFactory::addSpriteRenderer(ObjectFormat& obj, GameObject& go, sf::RenderWindow& window)
{
    //std::string textureKey = Parser::getTexture(obj);
    std::string textureKey = "PaddleBase2.png";
    AssetManager::getInstance().loadTexture(textureKey, textureKey);
    auto spriteComp = go.addComponent<SpriteRenderComponent>(go,
                                                             window,
                                                             AssetManager::getInstance().getTexture(textureKey),
                                                             "GameObjects",
                                                             sf::IntRect(0, 0, 0, 0));
    auto  textureSize = AssetManager::getInstance().getTexture(textureKey).getSize();
    float scale       = Parser::getSize(obj).x / textureSize.x;
    spriteComp->getSprite().setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);
    spriteComp->setScale(scale, scale);
}

b2FixtureDef GameObjectFactory::createFixtureDef(ObjectFormat& obj)
{
    const auto   size      = PhysicsManager::s2b(Parser::getSize(obj));
    const auto   shapeType = Parser::getShape(obj);
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
    fixtureDef.density  = Parser::getDensity(obj);
    fixtureDef.isSensor = Parser::isSensor(obj);

    return fixtureDef;
}
} // namespace mmt_gd