#pragma once

#include "GameObject.hpp"
#include <SFML/Graphics.hpp>

namespace mmt_gd
{
    class GameObjectFactory
    {
    public:
        static GameObject::Ptr createPuck(sf::RenderWindow& window);
        static GameObject::Ptr createPaddle(sf::RenderWindow& window);
        static GameObject::Ptr createWall(sf::RenderWindow& window);
        static GameObject::Ptr createNeutralzone(sf::RenderWindow& window);
        static GameObject::Ptr createGoalsensor(sf::RenderWindow& window);
        static GameObject::Ptr createGoalbarrier(sf::RenderWindow& window);
    };
} // namespace mmt_gd