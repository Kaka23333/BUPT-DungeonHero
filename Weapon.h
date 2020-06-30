#pragma once
#include <SFML/Graphics.hpp>
#include "Bullet.h"

class Weapon {
public:
	int shot_speed{ 10 };        //�ӵ������ٶ�
	int damage{ 1 };            //�ӵ��˺�
	double bullet_delay{ 0.01 };  //�ӵ�ÿ�����
	int range{ 400 };           //�ӵ����
	int x, y;
	std::deque<Bullet> bullets{};  //�ӵ�XX����ר��
	sf::Texture image;//ͼƬ
	sf::Sprite bullet;//�ӵ�����

public:
	Weapon(int x_, int y_, int damage_);
	void SetXY(int x_, int y_);//�ӿ�
	void check();//��̿���;
};