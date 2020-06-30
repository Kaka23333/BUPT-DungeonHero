#pragma once
#include <SFML/Graphics.hpp>

class Bullet {
public:
	int x;
	int y;
	sf::Vector2f direction{ 0, 0 };
	int coord_x;
	int coord_y;//Éä³öµã×ø±ê
};