#pragma once
#include <SFML/Graphics.hpp>
#include <ctime>
#include <deque>
#include <iostream>
#include <string>
extern sf::RenderWindow window;

//Ӣ��
const int size = 16;
const int windowWidth = 60;
const int windowHeight = 40;

class Hero {
 public:
  int dir;  //����
  float x{21}, y{10};
  int code;      //���� 1��ʦ 2 ������ʿ
  int HP = 100;  //Ѫ��
  bool isLive = true;
  bool fire = false;
  short int speed = 8;    //ÿ֡λ��//�ٶ�
  short int RunType = 0;  //�ܶ���Ч
  std::string image;      //ͼ��
  std::string getTexture() { return image; }
  Hero() {}
  Hero(int x_, int y_, int code_ = 1) : x(x_), y(y_), code(code_) {}
  int getHp() const { return HP; }
  void setHp(int Hp) { this->HP = Hp; }
  int getWeaponLeve() const {}
  void setWeaponLeve(int level);
  int getDirect() const;
  void setDirection(int dir);
  void launch();
  void renderBorder();
  void renderHero();
  void update();
  Mutex heroMutex;
  sf::Vector2f getCoord() { return sf::Vector2f{x, y}; }
  //����: 0-x 1-y 2-code 3-direaction 4-Hp
  int& operator[](int CODE_DIR_Hp);
};
//����: 0-x 1-y 2-code 3-direaction 4-Hp
int& Hero::operator[](int CODE_DIR_Hp) {
  switch (CODE_DIR_Hp) {
    case 0:
      return code;
      break;
    case 1:
      return dir;
      break;
    case 2:
      return HP;
      break;
    default:;
      break;  //
  }
}
void Hero::launch() {  //����
  std::cout << "��ʼ!";
  dir = 3;  //����Ϊ��
}

void Hero::renderBorder() {  //��ײ���
}

void Hero::update() {  //�ƶ�
                       //��ʦ�ƶ�
  if (dir == 2) x -= speed;
  if (dir == 1) y += speed;
  if (dir == 3) x += speed;
  if (dir == 0) y -= speed;
  if (x >= windowWidth - 1) isLive = false;
  if (x < 1) isLive = false;
  if (y >= windowHeight - 1) isLive = false;
  if (y < 1) isLive = false;
}
void Hero::renderHero() {  //��ͼ����
  heroMutex.lock();
  if (2 == dir || 0 == dir)  // 0��1��2��3��
  {
    if (++RunType % 2)
      image = "images/mleft1.png";
    else
      image = "images/mleft2.png";
  } else {
    if (++RunType % 2)
      image = "images/mright1.png";
    else
      image = "images/mright2.png";
  }
  heroMutex.unlock();
}
