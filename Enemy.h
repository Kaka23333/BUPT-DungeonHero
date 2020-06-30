#pragma once
#include <string>
#include <vector>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
using namespace sf;

class Enemy {
public:
	Enemy(int id_);//接受种类来初始化
	void beHitted(int damage_);//受到攻击时
	void enemyMove();//随机攻击和移动
private:
	int hp{ 100 };  //生命
	int id{ 1 };  //种类
	int speed{ 2 };  //速度
	bool exist{ true };  //存在
	int crood_x{ 0 };  //x,y坐标
	int crood_y{ 0 };
	std::string picturepath[2][2];  //贴图
	int currentdirect{ 0 };  //方向：0上，1下，2左，3右
	bool shoot{ false };
};

class eBullet {
public:
	eBullet(int x_, int y_, int direction_);
	FloatRect eBuletBox;
	int x{ 0 }, y{ 0 };             //射出点坐标
	int direction{ 0 };             // 0上，1下，2左，3右
	int shot_speed{ 5 };
	int range{ 1500 };
	int pass{ 0 };
	bool exist = true;
};
