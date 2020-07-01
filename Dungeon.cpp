#include "enemy.h"
#include "hero.h"
#include "shoot.h"
using namespace sf;

sf::RenderWindow window;

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
    window.draw(mainMenu);

    Event mouse;

    while (window.isOpen()) {
        if (window.pollEvent(mouse))  // 检查是否有事件发生
        {
            if (mouse.type == Event::MouseMoved)  // 鼠标碰到按钮
                // 开始
                if (mouse.mouseMove.x > 600 && mouse.mouseMove.x < 792 &&
                    mouse.mouseMove.y > 336 && mouse.mouseMove.y < 432)
                    window.draw(start);
            // 设置
                else if (mouse.mouseMove.x > 600 && mouse.mouseMove.x < 816 &&
                    mouse.mouseMove.y > 504 && mouse.mouseMove.y < 600)
                    window.draw(setting);
            // 帮助
                else if (mouse.mouseMove.x > 600 && mouse.mouseMove.x < 816 &&
                    mouse.mouseMove.y > 672 && mouse.mouseMove.y < 768)
                    window.draw(help);
            // 退出
                else if (mouse.mouseMove.x > 600 && mouse.mouseMove.x < 816 &&
                    mouse.mouseMove.y > 840 && mouse.mouseMove.y < 912) {
                    window.draw(exit);
                }
            // 没有碰到按钮
                else
                    window.draw(mainMenu);
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
            window.display();
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

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        for (int i = 0; i < 20; i++) {
            if (0 == i || 19 == i)
                for (int j = 3; j < 30; j++) {
                    block.setPosition(j * 48, i * 48);
                    window.draw(block);
                }
            else {
                block.setPosition(144, i * 48);
                window.draw(block);
                block.setPosition(1392, i * 48);
                window.draw(block);
            }
        }

        window.display();
    }
}



// 碰撞检测函数
bool checkCollision(Sprite s1, Sprite s2) {
    return s1.getLocalBounds().intersects(s2.getLocalBounds());
}


int main() {
    typedef sf::Keyboard sk;           //重要sf::Keyboard sk 简写

    window.create(sf::VideoMode(1440, 900), "Dumbledore's adventures");

    Enemy e1{ 1 }, e2{ 2 }, e3{ 1 };

    Hero h1;
    Weapon b1;
    Thread shootThread2{ &Weapon::shoot_mouse, &b1 };
    //Thread heroThread1{ &Hero::move, &h1 };
    Thread enemyThread1{ &Enemy::enemyMove, &e1 };
    Thread enemyThread2{ &Enemy::enemyMove, &e2 };
    Thread enemyThread3{ &Enemy::enemyMove, &e3 };

    //heroThread1.launch();
    shootThread2.launch();
    enemyThread1.launch();
    enemyThread2.launch();
    enemyThread3.launch();

    bool press = false;
    Texture t;
    Sprite s, s1, s2, s3, s4, ebullet;

    sf::Clock clock;
    float dtTime = 0;
    float delay = 0.01;  // tmd
    //自定义帧率
    h1.launch();

    Event e;

    while (window.isOpen()) {
        window.pollEvent(e);

        // 射击并打印主角子弹
        if (b1.getFire()) {
            if (e.type == sf::Event::MouseButtonPressed || press)  // 鼠标按住
            {
                sf::Vector2f mouse{ sf::Mouse::getPosition(window) };
                b1.setDirection(mouse, h1);
                press = true;
            }
            if (e.type == sf::Event::MouseButtonReleased)  //鼠标释放
                press = false;
        }
        t.loadFromFile(b1.getTexture());
        s.setTexture(t);
        for (int i = 0; b1.bullets.size() > i; i++) {
            s.setPosition(b1.bullets[i].getCoord());
            window.draw(s);
        }

        // 主角部分
        dtTime += clock.restart().asSeconds();
        if (e.type == sf::Event::Closed) window.close();
        switch (e.key.code) {  //上0下1左2右三
        case sk::Left:
            h1.dir = 2;
            break;  //方向控制
        case sk::Down:
            h1.dir = 1;
            break;
        case sk::Right:
            h1.dir = 3;
            break;
        case sk::Up:
            h1.dir = 0;
            break;
        }

        if (dtTime > delay) {  //重要的时间间隔刷新函数
            dtTime = 0;
            h1.update();  //逻辑移动
        }

        //图像移动
        h1.renderHero();
        t.loadFromFile(h1.getTexture());
        s4.setTexture(t);
        s4.setPosition(h1.getCoord());
        window.draw(s4);

        // 敌人
        t.loadFromFile(e1.getTexture());
        s1.setTexture(t);
        s1.setPosition(e1.getCoord());
        window.draw(s1);

        t.loadFromFile(e3.getTexture());
        s2.setTexture(t);
        s2.setPosition(e3.getCoord());
        window.draw(s2);

        t.loadFromFile(e2.getTexture());
        s3.setTexture(t);
        s3.setPosition(e2.getCoord());
        window.draw(s3);

        // 敌人子弹
        t.loadFromFile("images/bullet4.png");
        ebullet.setTexture(t);
        for (int i = 0; i < e2.ebullets.size(); i++) {
            if (e2.ebullets[i].getExist()) {
                ebullet.setPosition(e2.ebullets[i].getCoord());
                window.draw(ebullet);
            }
        }

        // 打印到屏幕上
        window.display();
        window.clear();
        Sleep(5);
    }

    return 0;
}

