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
  bool isLive = true;
  bool fire = false;
  short int speed = 4;    //ÿ֡λ��//�ٶ�
  short int RunType = 0;  //�ܶ���Ч
  std::string image;      //ͼ��
  std::string getTexture() { return image; }
  Hero() {}
  Hero(int x_, int y_) : x(x_), y(y_) {}
  void launch();
  void renderBorder();
  void renderHero();
  void update();
  void move();
  Mutex heroMutex;
  sf::Vector2f getCoord() { return sf::Vector2f{ x, y }; }
};

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

void Hero::move() {
  sf::Clock clock;
  float dtTime = 0;
  float delay = 0.01;  // tmd
  //�Զ���֡��
  launch();
  sf::Event event;
  while (window.isOpen()) {  // A!S!d!w!

    dtTime += clock.restart().asSeconds();

    typedef sf::Keyboard sk;           //��Ҫsf::Keyboard sk ��д
    while (window.pollEvent(event)) {  //�¼���ѯ
      if (event.type == sf::Event::Closed) window.close();
      switch (event.key.code) {  //��0��1��2����
        case sk::A:
          dir = 2;
          break;  //�������
        case sk::S:
          dir = 1;
          break;
        case sk::D:
          dir = 3;
          break;
        case sk::W:
          dir = 0;
          break;
      }
    }

    if (dtTime > delay) {  //��Ҫ��ʱ����ˢ�º���
      dtTime = 0;
      update();  //�߼��ƶ�
    }

    //renderBorder();  //ͼ���ƶ�
    renderHero();

  }
}
