#pragma once
#include "Hero.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <deque>
#include <iostream>
#include <string>
#include <windows.h>

sf::Mutex myMutex_;

class Bullet {
public:
    sf::Vector2f xy{ 0, 0 }; //���������
    sf::Vector2f direction{ 0, 0 };
    sf::Vector2f coord{ 0, 0 }; //���������
    sf::Vector2f getCoord() { return coord; }
    bool exist = true;
    bool getExist() { return exist; }
    void setExist(bool x) { exist = x; }
};

class Weapon {
private:
    float timer{ 0.0 };
    int shot_speed{ 4 };        //�ӵ������ٶ�
    int damage{ 1 };            //�ӵ��˺�
    double bullet_delay{ 0.2 }; //�ӵ�ÿ�����
    int range{ 1 };             //�ӵ����
    bool isFire = true;
    sf::Vector2f point{ 500, 500 };
    std::string t1, t2, t3, bullet;

public:
    Hero hero;
    Weapon();
    void check();
    void shoot_mouse();

    void setDirection(sf::Vector2f, Hero&, const float delta);
    void setFire(bool isFire_) { isFire = isFire; }

    bool getFire() { return isFire; }
    int getDamage() { return damage; }
    void setDamage(int damage_) { damage = damage_; }
    std::string getTexture() { return bullet; }

    std::deque<Bullet> bullets{}; //�ӵ�
};
Weapon::Weapon() {
    t1 = "images/bullet1.png";
    t2 = "images/bullet2.png";
    t3 = "images/bullet3.png";
}

//����ӵ���״��
void Weapon::check() {
    for (int i = 0; i < bullets.size(); i++) {
        if (pow(bullets[i].coord.x - bullets[i].xy.x, 2) +
            pow(bullets[i].coord.y - bullets[i].xy.y, 2) >
            range* range) {
            bullets[i].exist = false;
            // else if (bullets[i].coord.x) ײ������
            // else if (bullets[i].coord.x) ײ����
        }
    }
}

void Weapon::setDirection(sf::Vector2f mouse, Hero& hero_, const float delta) {
    // ��������ƫ������ʹ�ӵ��ӷ��������//��0��1��2����
    point = hero_.getCoord() + sf::Vector2f{ delta, 20 };
    sf::Vector2f temp_xy{ mouse };
    float temp =
        pow(shot_speed * shot_speed /
        (pow(temp_xy.x - point.x, 2) + pow(temp_xy.y - point.y, 2)),
            0.5);
    myMutex_.lock();
    bullets.push_back(Bullet{
        point,
        sf::Vector2f{(temp_xy.x - point.x) * temp, (temp_xy.y - point.y) * temp},
        point });
    timer = 0;
    isFire = false;
    myMutex_.unlock();
}

void Weapon::shoot_mouse() {
    sf::Clock clock;
    sf::Event e;
    bool press = false;

    while (1) {
        auto time{ clock.restart().asSeconds() };
        timer += time;

        if (timer > bullet_delay)
            isFire = true;

        //�ӵ����˺��ı�
        switch (damage) {
        case 1:
            bullet = t1;
            break;
        case 2:
            bullet = t2;
            break;
        case 3:
            bullet = t3;
        }
        myMutex_.lock();
        for (int i = 0; bullets.size() > i; i++) {
            bullets[i].coord += bullets[i].direction;
        }
        myMutex_.unlock();
        Sleep(10);
    }
};
