#pragma once
#include <string>
#include <vector>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
using namespace sf;

class Enemy {
public:
	Enemy(int id_);//������������ʼ��
	void beHitted(int damage_);//�ܵ�����ʱ
	void enemyMove();//����������ƶ�
private:
	int hp{ 100 };  //����
	int id{ 1 };  //����
	int speed{ 2 };  //�ٶ�
	bool exist{ true };  //����
	int crood_x{ 0 };  //x,y����
	int crood_y{ 0 };
	std::string picturepath[2][2];  //��ͼ
	int currentdirect{ 0 };  //����0�ϣ�1�£�2��3��
	bool shoot{ false };
};

class eBullet {
public:
	eBullet(int x_, int y_, int direction_);
	FloatRect eBuletBox;
	int x{ 0 }, y{ 0 };             //���������
	int direction{ 0 };             // 0�ϣ�1�£�2��3��
	int shot_speed{ 5 };
	int range{ 1500 };
	int pass{ 0 };
	bool exist = true;
};
