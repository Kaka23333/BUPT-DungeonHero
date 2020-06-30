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
std::deque<Bullet> bullets{};  //子弹
std::vector<eBullet> ebullets{};///////////////////////////////大子弹

using namespace sf;

// 菜单界面
int menu() {
    // 创建纹理对象
    Texture mainMenu_, start_, setting_, help_, exit_;
    // 初始化纹理
    mainMenu_.loadFromFile("images/mainMenu.png");
    start_.loadFromFile("images/start.png");
    setting_.loadFromFile("images/setting.png");
    help_.loadFromFile("images/help.png");
    exit_.loadFromFile("images/exit.png");
    // 创建精灵对象
    Sprite mainMenu(mainMenu_), start(start_), setting(setting_),
        help(help_), exit(exit_);
    // 打印菜单界面
    window_.draw(mainMenu);

    Event mouse;

    while (window_.isOpen()) {
        if (window_.pollEvent(mouse))  // 检查是否有事件发生
        {
            if (mouse.type == Event::MouseMoved)  // 鼠标碰到按钮
                // 开始
                if (mouse.mouseMove.x > 600 && mouse.mouseMove.x < 792 &&
                    mouse.mouseMove.y > 336 && mouse.mouseMove.y < 432)
                    window_.draw(start);
                // 设置
                else if (mouse.mouseMove.x > 600 && mouse.mouseMove.x < 816 &&
                    mouse.mouseMove.y > 504 && mouse.mouseMove.y < 600)
                    window_.draw(setting);
                // 帮助
                else if (mouse.mouseMove.x > 600 && mouse.mouseMove.x < 816 &&
                    mouse.mouseMove.y > 672 && mouse.mouseMove.y < 768)
                    window_.draw(help);
                // 退出
                else if (mouse.mouseMove.x > 600 && mouse.mouseMove.x < 816 &&
                    mouse.mouseMove.y > 840 && mouse.mouseMove.y < 912) {
                    window_.draw(exit);
                }
                // 没有碰到按钮
                else
                    window_.draw(mainMenu);
            if (mouse.type == Event::MouseButtonPressed)  // 鼠标点击按钮
                // 开始
                if (mouse.mouseButton.x > 600 && mouse.mouseButton.x < 792 &&
                    mouse.mouseButton.y > 336 && mouse.mouseButton.y < 432 &&
                    mouse.mouseButton.button == Mouse::Button::Left)
                    return 1;
                // 设置
                else if (mouse.mouseButton.x > 600 && mouse.mouseButton.x < 816 &&
                    mouse.mouseButton.y > 504 && mouse.mouseButton.y < 600 &&
                    mouse.mouseButton.button == Mouse::Button::Left)
                    return 2;
                // 帮助
                else if (mouse.mouseButton.x > 600 && mouse.mouseButton.x < 816 &&
                    mouse.mouseButton.y > 672 && mouse.mouseButton.y < 768 &&
                    mouse.mouseButton.button == Mouse::Button::Left)
                    return 3;
                // 退出
                else if (mouse.mouseButton.x > 600 && mouse.mouseButton.x < 8167 &&
                    mouse.mouseButton.y > 840 && mouse.mouseButton.y < 912 &&
                    mouse.mouseButton.button == Mouse::Button::Left)
                    return 0;
            // 显示所画的图片
            window_.display();
        }
    }
}

// 房间函数
void room() {
    // 初始化边界块
    Texture block_;
    block_.loadFromFile("images/block.png");
    // 初始化精灵
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



// 怪物多线程测试代码
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