#include <deque>
#include "Weapon.h"
#include "Bullet.h"

extern std::deque<Bullet> bullets;
extern sf::RenderWindow window_;

Weapon::Weapon() {
    t1.loadFromFile("images/bullet1.png");
    t2.loadFromFile("images/bullet2.png");
    t3.loadFromFile("images/bullet3.png");
    bullet.setTexture(t1);
}

//检测子弹的状况
void Weapon::check() {
    for (std::deque<Bullet>::iterator iter = bullets.begin();
        iter != bullets.end();) {
        if (pow(iter->coord.x - iter->xy.x, 2) +
            pow(iter->coord.y - iter->xy.y, 2) >
            range* range) {
            bullets.pop_front();  //超出射程
                                  // else if (bullets[i].coord.x) 撞到地形
                                  // else if (bullets[i].coord.x) 撞到怪
        }
    }
}

void Weapon::shoot_mouse() {
    sf::Clock clock;
    sf::Event e;
    auto timer{ 0.0 };
    bool shoot_judge = true;
    bool press = false;

    while (window_.isOpen()) {
        auto time{ clock.restart().asSeconds() };
        timer += time;

        if (timer > bullet_delay) shoot_judge = true;

        //产生新子弹
        window_.pollEvent(e);
        if (shoot_judge) {
            if (e.type == sf::Event::MouseButtonPressed || press)  // 鼠标按住
            {
                sf::Vector2f temp_xy{ sf::Mouse::getPosition(window_) };
                float temp =
                    pow(shot_speed * shot_speed /
                    (pow(temp_xy.x - point.x, 2) + pow(temp_xy.y - point.y, 2)),
                        0.5);
                bullets.push_back(Bullet{ point,
                                         sf::Vector2f{(temp_xy.x - point.x) * temp,
                                                      (temp_xy.y - point.y) * temp},
                                         point });
                timer = 0;
                shoot_judge = false;
                press = true;
            }
        }
        if (e.type == sf::Event::MouseButtonReleased)  //鼠标释放
            press = false;

        //子弹随伤害改变
        switch (damage) {
        case 1:
            bullet.setTexture(t1);
            break;
        case 2:
            bullet.setTexture(t2);
            break;
        case 3:
            bullet.setTexture(t3);
        }

        for (int i = 0; bullets.size() > i; i++) {
            bullets[i].coord += bullets[i].direction;
            bullet.setPosition(bullets[i].coord);
            window_.draw(bullet);
        }
        window_.display();
        window_.clear();
        check();
    }
};