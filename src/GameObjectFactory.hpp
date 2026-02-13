//This code was made for the Multimedia Project 2a for the Multimedia Technology class at Fachhochschule Salzburg, by Pascal Veselka & Pascal Marsche
#pragma once

#include "GameObject.hpp"
#include <SFML/Graphics.hpp>
#include "Tileson.hpp"


namespace mmt_gd
{
    class GameObjectFactory
    {
    using ObjectFormat = tson::Object;

    public:
        static void createGameObject(sf::RenderWindow& window, ObjectFormat& obj);
        static GameObject::Ptr createPuck(sf::RenderWindow& window, ObjectFormat& obj);
        static GameObject::Ptr createPaddle(sf::RenderWindow& window, ObjectFormat& obj);
        static GameObject::Ptr createWall(sf::RenderWindow& window, ObjectFormat& obj);
        static GameObject::Ptr createNeutralzone(sf::RenderWindow& window, ObjectFormat& obj);
        static GameObject::Ptr createExtraWall(sf::RenderWindow& window, ObjectFormat& obj);
        static GameObject::Ptr createPenaltyarea(sf::RenderWindow& window, ObjectFormat& obj);
        static GameObject::Ptr createGoalsensor(sf::RenderWindow& window, ObjectFormat& obj);
        static GameObject::Ptr createGoalbarrier(sf::RenderWindow& window, ObjectFormat& obj);
        static GameObject::Ptr createPuckSpawn(sf::RenderWindow& window, ObjectFormat& obj);

    private:
        static GameObject::Ptr createObject(ObjectFormat& obj);
        static void addSpriteRenderer(ObjectFormat& obj, GameObject& go, sf::RenderWindow& window);
        static b2FixtureDef createFixtureDef(ObjectFormat& obj);
    };


    enum CollisionLayers
    {
        WALL            = 0x0001,
        FAKE_WALL       = 0x0002,
        PENALTY         = 0x0004,
        GOAL_SENSOR     = 0x0008,
        OBJECTS         = 0x0010,
        PUCK_WALL       = 0x0020,
        PLAYER_ALL      = 0x0040,
        PLAYER_1        = 0x0080,
        PLAYER_2        = 0x0100
    };


} // namespace mmt_gd