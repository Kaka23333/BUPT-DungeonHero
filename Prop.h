#pragma once
#include <SFML/Graphics.hpp>
#include "Block.h"
#include <list>
#include <string>
#include <random>
#include <queue>
#include"Counter.h"
using std::string;



//类
class Box {//宝箱
	float x{ 0 };
	float y{ 0 };
	bool state{ 1 };//未打卡为1
	sf::Sprite cimage;//未打开
	sf::Sprite oimage;//已打开
public:
	Box() = default;
	Box(float x_, float y_);
	float getx();
	float gety();
	sf::Sprite getCimage();
	sf::Sprite getOimage();
	bool getState();
	void open();//打开宝箱
	void drop();//道具掉落
};


class Prop {//道具类
protected:
	float x{ 0 }; //坐标x
	float y{ 0 }; //坐标y
	int mode{2}; //道具种类 1:fheal 2:pheal 3:bomb 4:lvup 5:pause
	sf::Sprite image;
public:
	Prop() = default;
	Prop(float x_, float y_, int mode_);
	sf::Sprite getImage();
	int getMode();
};


class Bomb : public Prop {
	int no_;//编号
public:
	Bomb(int x_, int y_, int num);
	int getNo();
	int getx();
	int gety();
	void setPos(int x_, int y_);
};


class PropMenu {//道具栏结构体
public:
	int num{ 0 };
	bool isChosen{ 0 };
	void reset();
};



//函数
void textureInit();//初始化所有图片纹理
void boxInit();//初始化宝箱位置
void boxShow();//画宝箱
void propShow(); //显示掉落道具
void propMenuShow();//展示物品栏*
void prop_to_master();//使用对主角生效的道具
void prop_to_monster(int x, int y);//使用对小怪生效的道具，参数为鼠标点击位置
void effectShow();//炸弹特效