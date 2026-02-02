#pragma once

#include "GameObject.hpp"
#include <SFML/Graphics.hpp>

namespace mmt_gd
{
    class GameObjectFactory
    {
    public:
        static GameObject::Ptr createPuck(sf::RenderWindow& window, tson::Object& obj);
        static GameObject::Ptr createPaddle(sf::RenderWindow& window, tson::Object& obj);
        static GameObject::Ptr createWall(sf::RenderWindow& window, tson::Object& obj);
        static GameObject::Ptr createNeutralzone(sf::RenderWindow& window, tson::Object& obj);
        static GameObject::Ptr createPenaltyarea(sf::RenderWindow& window, tson::Object& obj);
        static GameObject::Ptr createGoalsensor(sf::RenderWindow& window, tson::Object& obj);
        static GameObject::Ptr createGoalbarrier(sf::RenderWindow& window, tson::Object& obj);

    private:
        static GameObject::Ptr createObject(tson::Object& obj);
        static void addSpriteRenderer(tson::Object& obj, GameObject& go, sf::RenderWindow& window);
        static b2FixtureDef createFictureDef(tson::Object& obj);
        static sf::Vector2f t2s(tson::Vector2i vec);
    };


    enum CollisionLayers
    {
        WALL            = 0x0000,
        FAKE_WALL       = 0x0001,
        PENALTY         = 0x0002,
        GOAL_SENSOR     = 0x0004,
        OBJECTS         = 0x0008
    };


} // namespace mmt_gd