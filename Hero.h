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
	short int HeroSpeed = 4;//ÿ֡λ��//�ٶ�
	short int RunType = 0;//�ܶ���Ч
	int HreoDirection = 3;//����
	sf::Texture image;//ͼ��
	Weapon wand{ x,y ,1 };//���� magic stick hhh ħ�� ,��ʼλ�ú��˺�
	Hero(int x_, int y_) : x(x_), y(y_) {}
	void launch() {//����
		std::cout << "��ʼ!";
		HreoDirection = 3;//����Ϊ��
	}
};
