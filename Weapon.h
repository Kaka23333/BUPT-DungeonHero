#pragma once
#include <SFML/Graphics.hpp>
#include "Bullet.h"

class Weapon {
public:
	int shot_speed{ 10 };        //子弹飞行速度
	int damage{ 1 };            //子弹伤害
	double bullet_delay{ 0.01 };  //子弹每发间隔
	int range{ 400 };           //子弹射程
	int x, y;
	std::deque<Bullet> bullets{};  //子弹XX武器专属
	sf::Texture image;//图片
	sf::Sprite bullet;//子弹精灵

public:
	Weapon(int x_, int y_, int damage_);
	void SetXY(int x_, int y_);//接口
	void check();//射程控制;
};