#pragma once
#include <SFML/Graphics.hpp>
#include <ctime>
#include <deque>
#include <iostream>
#include "Weapon.h"
#define DLX 80

class Hero {
public:
	int x, y;
	bool isLive = true;
	bool fire = false;
	short int dlx = DLX;
	short int HeroSpeed = 4;//每帧位移//速度
	short int RunType = 0;//跑动特效
	int HreoDirection = 3;//方向
	sf::Texture image;//图像
	Weapon wand{ x,y ,1 };//武器 magic stick hhh 魔杖 ,起始位置和伤害
	Hero(int x_, int y_) : x(x_), y(y_) {}
	void launch() {//启动
		std::cout << "开始!";
		HreoDirection = 3;//方向为右
	}
};
