#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <deque>
#include "Bullet.h"
#include "Weapon.h"
#include "Enemy.h"

RenderWindow window_;
Mutex myMutex;
std::deque<Bullet> bullets{};  //�ӵ�
std::vector<eBullet> ebullets{};///////////////////////////////���ӵ�

using namespace sf;

// �˵�����
int menu() {
    // �����������
    Texture mainMenu_, start_, setting_, help_, exit_;
    // ��ʼ������
    mainMenu_.loadFromFile("images/mainMenu.png");
    start_.loadFromFile("images/start.png");
    setting_.loadFromFile("images/setting.png");
    help_.loadFromFile("images/help.png");
    exit_.loadFromFile("images/exit.png");
    // �����������
    Sprite mainMenu(mainMenu_), start(start_), setting(setting_),
        help(help_), exit(exit_);
    // ��ӡ�˵�����
    window_.draw(mainMenu);

    Event mouse;

    while (window_.isOpen()) {
        if (window_.pollEvent(mouse))  // ����Ƿ����¼�����
        {
            if (mouse.type == Event::MouseMoved)  // ���������ť
                // ��ʼ
                if (mouse.mouseMove.x > 600 && mouse.mouseMove.x < 792 &&
                    mouse.mouseMove.y > 336 && mouse.mouseMove.y < 432)
                    window_.draw(start);
                // ����
                else if (mouse.mouseMove.x > 600 && mouse.mouseMove.x < 816 &&
                    mouse.mouseMove.y > 504 && mouse.mouseMove.y < 600)
                    window_.draw(setting);
                // ����
                else if (mouse.mouseMove.x > 600 && mouse.mouseMove.x < 816 &&
                    mouse.mouseMove.y > 672 && mouse.mouseMove.y < 768)
                    window_.draw(help);
                // �˳�
                else if (mouse.mouseMove.x > 600 && mouse.mouseMove.x < 816 &&
                    mouse.mouseMove.y > 840 && mouse.mouseMove.y < 912) {
                    window_.draw(exit);
                }
                // û��������ť
                else
                    window_.draw(mainMenu);
            if (mouse.type == Event::MouseButtonPressed)  // �������ť
                // ��ʼ
                if (mouse.mouseButton.x > 600 && mouse.mouseButton.x < 792 &&
                    mouse.mouseButton.y > 336 && mouse.mouseButton.y < 432 &&
                    mouse.mouseButton.button == Mouse::Button::Left)
                    return 1;
                // ����
                else if (mouse.mouseButton.x > 600 && mouse.mouseButton.x < 816 &&
                    mouse.mouseButton.y > 504 && mouse.mouseButton.y < 600 &&
                    mouse.mouseButton.button == Mouse::Button::Left)
                    return 2;
                // ����
                else if (mouse.mouseButton.x > 600 && mouse.mouseButton.x < 816 &&
                    mouse.mouseButton.y > 672 && mouse.mouseButton.y < 768 &&
                    mouse.mouseButton.button == Mouse::Button::Left)
                    return 3;
                // �˳�
                else if (mouse.mouseButton.x > 600 && mouse.mouseButton.x < 8167 &&
                    mouse.mouseButton.y > 840 && mouse.mouseButton.y < 912 &&
                    mouse.mouseButton.button == Mouse::Button::Left)
                    return 0;
            // ��ʾ������ͼƬ
            window_.display();
        }
    }
}

// ���亯��
void room() {
    // ��ʼ���߽��
    Texture block_;
    block_.loadFromFile("images/block.png");
    // ��ʼ������
    Sprite block(block_);
    block.setScale(0.08f, 0.08f);

    while (window_.isOpen())
    {
        sf::Event event;
        while (window_.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window_.close();
        }

        window_.clear();
        for (int i = 0; i < 20; i++) {
            if (0 == i || 19 == i)
                for (int j = 3; j < 30; j++) {
                    block.setPosition(j * 48, i * 48);
                    window_.draw(block);
                }
            else {
                block.setPosition(144, i * 48);
                window_.draw(block);
                block.setPosition(1392, i * 48);
                window_.draw(block);
            }
        }
        window_.display();
    }
}

int main() {
    window_.create(VideoMode(1440, 960), "game");

    menu();

    return 0;
}



// ������̲߳��Դ���
/*
    Enemy e1{ 1 }, e2{ 2 }, e3{ 1 };

    window_.setActive(false);

    Thread enemyThread1{ &Enemy::enemyMove, &e1 };
    Thread enemyThread2{ &Enemy::enemyMove, &e2 };
    Thread enemyThread3{ &Enemy::enemyMove, &e3 };

    enemyThread1.launch();
    enemyThread2.launch();
    enemyThread3.launch();
*/