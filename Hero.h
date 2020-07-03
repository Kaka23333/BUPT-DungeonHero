#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <deque>
#include <iostream>
#include <string>
using namespace sf;
extern int nEnemy;
//Ӣ��
const int size = 16;
const int windowWidth = 60;
const int windowHeight = 40;

// ���ڶ��߳�ԭ��Ӣ���ٶ��淿���ڹ����������ݼ�
// ��6������Ϊ��
// ʣ��6����heroSpeed = 14  7.32s
// ʣ��5����heroSpeed = 12  7.34s
// ʣ��4����heroSpeed = 10  7.42s
// ʣ��3����heroSpeed = 8   7.51s
// ʣ��2����heroSpeed = 7   7.54s
// ʣ��1����heroSpeed = 5   7.75s
// ʣ��0����heroSpeed = 3.5 7.58s

class Hero {
public:
    int dir; //����
    float x{ 201 }, y{ 100 };
    int code; //���� 1��ʦ 2 ������ʿ
    int HP;   //Ѫ��
    bool isLive = true;
    bool fire = false;
    float heroSpeed = 14;                    //ÿ֡λ��//�ٶ�
    short int RunType = 0;                   //�ܶ���Ч
    std::string image = "images/mleft1.png"; //ͼ��
    std::string getTexture() { return image; }
    Hero() {
        x = 10.0;
        y = 10.0;
        code = 1;
    }
    Hero(int x_, int y_, int code_ = 1) : x(x_), y(y_), code(code_) { HP = 5; }
    int getHp() const { return HP; }
    void setHp(int Hp);
    int getWeaponLeve() const {}
    void setWeaponLeve(int level);
    int getDirect() const;
    void setDirection(int dir);
    void launch();
    void renderBorder();
    void renderHero();
    void update();
    void move();
    Mutex heroMutex;
    sf::Vector2f getCoord() { return sf::Vector2f{ x, y }; }
    //����: 0-x 1-y 2-code 3-direaction 4-Hp
    int& operator[](int CODE_DIR_Hp);
    void beHitted(int damage_);
    void restart() {
        x = 720;
        y = 700;
        HP = 100;
        isLive = true;
        fire = false;
    }
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
        break; //
    }
}
void Hero::launch() { //����
    std::cout << "��ʼ!";
    dir = 3; //����Ϊ��
}
void Hero::setHp(int HP) {
    if (HP > 100) {
        this->HP = 100;
    }
    else if (HP < 0) {
        this->HP = 0;
        this->isLive = 0;

    }
    else {
        this->HP = HP;
    }
}
void Hero::update() { //�ƶ�
  // ������߳���ɵ��ٶ����⣬����ʣ�����������Ӣ���ٶ�
    if (6 == nEnemy)
        heroSpeed = 14;
    if (5 == nEnemy)
        heroSpeed = 12;
    if (4 == nEnemy)
        heroSpeed = 10;
    if (3 == nEnemy)
        heroSpeed = 8;
    if (2 == nEnemy)
        heroSpeed = 7;
    if (1 == nEnemy)
        heroSpeed = 8;
    if (0 == nEnemy)
        heroSpeed = 10;
    //��ʦ�ƶ�
    if (dir == 2)
        x -= heroSpeed;
    if (dir == 1)
        y += heroSpeed;
    if (dir == 3)
        x += heroSpeed;
    if (dir == 0)
        y -= heroSpeed;
}
void Hero::renderHero() { //��ͼ����
    heroMutex.lock();
    switch (code) {
    case 1:
        if (2 == dir || 0 == dir) // 0��1��2��3��
        {
            if (++RunType % 2)
                image = "images/mleft1.png";
            else
                image = "images/mleft2.png";
        }
        else {
            if (++RunType % 2)
                image = "images/mright1.png";
            else
                image = "images/mright2.png";
        }
        break;
    case 2:
        if (2 == dir || 0 == dir) // 0��1��2��3��
        {
            if (++RunType % 2)
                image = "images/����1.png";
            else
                image = "images/����2.png";
        }
        else {
            if (++RunType % 2)
                image = "images/����3.png";
            else
                image = "images/����4.png";
        }
        break;
    case 3:
        if (2 == dir || 0 == dir) // 0��1��2��3��
        {
            if (++RunType % 2)
                image = "images/��1��1.png";
            else
                image = "images/��1��2.png";
        }
        else {
            if (++RunType % 2)
                image = "images/��1��3.png";
            else
                image = "images/��1��4.png";
        }
        break;
    default:;
        break;
    }
    heroMutex.unlock();
}

void Hero::beHitted(int damage_) {
    HP -= damage_;
    if (HP <= 0) {
        isLive = false;
    }
}
