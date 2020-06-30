#pragma once
#include <SFML/Graphics.hpp>

class Bullet {
public:
    sf::Vector2f xy{ 0, 0 };  //射出点坐标
    sf::Vector2f direction{ 0, 0 };
    sf::Vector2f coord{ 0, 0 };  //射出点坐标
};