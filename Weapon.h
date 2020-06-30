#pragma once
#include <SFML/Graphics.hpp>

class Weapon {
private:
    int shot_speed{ 4 };         //�ӵ������ٶ�
    int damage{ 1 };             //�ӵ��˺�
    double bullet_delay{ 0.2 };  //�ӵ�ÿ�����
    int range{ 800 };            //�ӵ����
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