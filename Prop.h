#pragma once
#include <SFML/Graphics.hpp>
#include "Block.h"
#include <list>
#include <string>
#include <random>
#include <queue>
#include"Counter.h"
using std::string;



//��
class Box {//����
	float x{ 0 };
	float y{ 0 };
	bool state{ 1 };//δ��Ϊ1
	sf::Sprite cimage;//δ��
	sf::Sprite oimage;//�Ѵ�
public:
	Box() = default;
	Box(float x_, float y_);
	float getx();
	float gety();
	sf::Sprite getCimage();
	sf::Sprite getOimage();
	bool getState();
	void open();//�򿪱���
	void drop();//���ߵ���
};


class Prop {//������
protected:
	float x{ 0 }; //����x
	float y{ 0 }; //����y
	int mode{2}; //�������� 1:fheal 2:pheal 3:bomb 4:lvup 5:pause
	sf::Sprite image;
public:
	Prop() = default;
	Prop(float x_, float y_, int mode_);
	sf::Sprite getImage();
	int getMode();
};


class Bomb : public Prop {
	int no_;//���
public:
	Bomb(int x_, int y_, int num);
	int getNo();
	int getx();
	int gety();
	void setPos(int x_, int y_);
};


class PropMenu {//�������ṹ��
public:
	int num{ 0 };
	bool isChosen{ 0 };
	void reset();
};



//����
void textureInit();//��ʼ������ͼƬ����
void boxInit();//��ʼ������λ��
void boxShow();//������
void propShow(); //��ʾ�������
void propMenuShow();//չʾ��Ʒ��*
void prop_to_master();//ʹ�ö�������Ч�ĵ���
void prop_to_monster(int x, int y);//ʹ�ö�С����Ч�ĵ��ߣ�����Ϊ�����λ��
void effectShow();//ը����Ч