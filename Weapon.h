#pragma once
#include <SFML/Graphics.hpp>

class Weapon {
private:
    int shot_speed{ 4 };         //子弹飞行速度
    int damage{ 1 };             //子弹伤害
    double bullet_delay{ 0.2 };  //子弹每发间隔
    int range{ 800 };            //子弹射程
    sf::Vector2f point{ 500, 500 };
	////
    sf::Texture t1, t2, t3;
    sf::Sprite bullet;

public:
    Weapon();
    void check();
    void shoot_mouse();
    void shoot_key();
};