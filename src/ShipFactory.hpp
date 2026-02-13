//This code was made for the Multimedia Project 2a for the Multimedia Technology class at Fachhochschule Salzburg, by Pascal Veselka & Pascal Marsche
#pragma once

#include "GameObject.hpp"
#include <SFML/Graphics.hpp>

namespace mmt_gd
{
class ShipFactory
{
public:
    static GameObject::Ptr createPlayerShip(sf::RenderWindow& window);
    static GameObject::Ptr createEnemyShip(sf::RenderWindow& window, GameObject* playerTarget);
};
} // namespace mmt_gd
