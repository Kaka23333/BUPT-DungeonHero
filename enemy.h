#pragma once
#include "hero.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <deque>
#include <random>
#include <string>
#include <windows.h>
using namespace sf;
extern int nEnemy;
extern std::vector<Vector2f> barrier; // ����ϰ���
extern bool isEffectPlay;

class eBullet {
public:
    eBullet(int x_, int y_, float dirx_);
    eBullet(int, int, float dirx_, float diry_);
    sf::Vector2f getCoord() { return sf::Vector2f{ x, y }; }
    bool getExist() { return exist; }
    float x{ 0 }, y{ 0 }; //���������
    float dir_x{ 0 };
    float dir_y{ 0 }; // 0�ϣ�1�£�2��3��
    int shot_speed{ 5 };
    int range{ 1000 };
    int pass{ 0 };
    int damage{ 10 };
    bool exist = true;
    void setExist(bool x) { exist = x; }
};

class Enemy {
public:
    Enemy();
    Enemy(int id_);             //������������ʼ��
    void beHitted(int damage_); //�ܵ�����ʱ
    void launch();
    void enemyStop() { stop = true; }
    void enemyMove(); //����������ƶ�
    void setSpeed(int speed_) { speed = speed_; }
    std::string getTexture() { return enemy; };
    sf::Vector2f getCoord() { return sf::Vector2f{ coord_x, coord_y }; }
    bool getShoot();
    bool getExist();
    int getDamge();

    std::deque<eBullet> ebullets{};
    bool stop{ false };
    bool exist{ true }; //����
    bool shoot{ true };
    int hp{ 100 };  //����
    int id{ 1 };    //����
    int speed{ 1 }; //�ٶ�
    int damage{ 10 };
    int currentdirect{ 0 }; //����0�ϣ�1�£�2��3��
    float coord_x{ 0 };     // x,y����
    float coord_y{ 0 };
    std::string enemy = "images/С��2������1.png";
    Texture t1, t2;
    Sprite block, temp;
};
Enemy::Enemy() {}
void Enemy::launch() {
    stop = false;
    exist = true;
    hp = 100;
    speed = 3;
    std::mt19937 rng;                 // 1300,900
    rng.seed(std::random_device()()); //��ʼ���������
    std::uniform_int_distribution<std::mt19937::result_type> x_(0, 1100); //
    std::uniform_int_distribution<std::mt19937::result_type> y_(0, 650);  //
    coord_x = x_(rng);
    coord_y = y_(rng); //��ʼ�����λ��
}

eBullet::eBullet(int x_, int y_, float dirx_) {
    std::mt19937 rng;                 // 1300,900
    rng.seed(std::random_device()()); //��ʼ���������
    std::uniform_int_distribution<std::mt19937::result_type> dir(0, 1100);
    x = x_;
    y = y_;
    dir_x = dirx_;
    if (dir(rng) % 2) {
        dir_y = sqrt(1 - dir_x * dir_x);
    }
    else {
        dir_y = 0 - sqrt(1 - dir_x * dir_x);
    }
}
eBullet::eBullet(int x_, int y_, float dirx_, float diry_) {
    x = x_;
    y = y_;
    dir_x = dirx_;
    dir_y = diry_;
};

Enemy::Enemy(int id_) : id{ id_ } {
    t1.loadFromFile("images/block.png");
    block.setTexture(t1);

    switch (id_) {
    case 1:
        shoot = false;
        break;
    case 2:
        shoot = true;
        break;
    } //���������ʼ������

    std::mt19937 rng;                 // 1300,900
    rng.seed(std::random_device()()); //��ʼ���������
    std::uniform_int_distribution<std::mt19937::result_type> x_(0, 1100); //
    std::uniform_int_distribution<std::mt19937::result_type> y_(0, 650);  //

    coord_x = x_(rng);
    coord_y = y_(rng); //��ʼ�����λ��
};

void Enemy::beHitted(int damage_) {
    hp -= damage_;
    if (hp <= 0) {
        exist = false;
        --nEnemy;
        if (nEnemy == 0 && isEffectPlay) {
            SoundBuffer bf;
            bf.loadFromFile("Sound effect/door heavy open.wav");
            Sound dooropen(bf);
            dooropen.play();
        }
    }
}

int Enemy::getDamge() { return damage; }
bool Enemy::getShoot() { return shoot; }
bool Enemy::getExist() { return exist; }

void Enemy::enemyMove() {

    sf::Mutex myMutex;
    Texture ebullet;
    Sprite bullet;
    ebullet.loadFromFile("images/bullet1.png");
    bullet.setTexture(ebullet);

    int futuredirect{ 0 };
    int allowdistance{ 0 };
    int distance{ 0 };
    int wantattack{ 0 };

    int lor = 0;     // 0 left 1right
    int picture = 0; // 1or2
    int add1tion{ 0 };

    std::mt19937 rng;                 // 1300,900
    rng.seed(std::random_device()()); //��ʼ���������
    std::uniform_int_distribution<std::mt19937::result_type> randomdirect(0, 3);
    std::uniform_int_distribution<std::mt19937::result_type> ifattack(0, 10);
    std::uniform_int_distribution<std::mt19937::result_type> distance_(0, 1960);
    std::uniform_int_distribution<std::mt19937::result_type> dir{ 0, 1000 };

    while (exist) {
        futuredirect = randomdirect(rng);
        currentdirect = futuredirect; //��ȡ����

        switch (currentdirect) {
        case 0:
            allowdistance = coord_y;
            break;
        case 1:
            allowdistance = 650 - coord_y;
            break;
        case 2:
            allowdistance = coord_x;
            lor = 0;
            break;
        case 3:
            allowdistance = 1100 - coord_x;
            lor = 1;
            break;
        } //�������ҵ�������
        if (allowdistance == 0) {
            continue;
        }
        if (stop == true) {
            switch (currentdirect) {
            case 0:
                currentdirect == 1;
                break;
            case 1:
                currentdirect == 0;
                break;
            case 2:
                currentdirect == 3;
                break;
            case 3:
                currentdirect == 2;
                break;
            }
            stop == false;
        }
        distance = distance_(rng) % allowdistance;
        if (speed != 0) {
            for (int i = 0; speed != 0 && i < distance / speed; i++) {
                if (stop == true) {
                    break;
                }
                wantattack = ifattack(rng);
                int it = static_cast<int>(ebullets.size());
                if (wantattack == 10 && this->shoot == true) {
                    if (it == 0 || ebullets[it - 1].pass >= 18 || !ebullets[it - 1].exist) {
                        if (lor == 0) {
                            ebullets.push_back(
                                eBullet(this->coord_x + 40, this->coord_y + 88,
                                    static_cast<float>(dir(rng) - 500) / 500.0));
                        }
                        else {
                            ebullets.push_back(
                                eBullet(this->coord_x + 170, this->coord_y + 88,
                                    static_cast<float>(dir(rng) - 500) / 500.0));
                        }
                    }
                }
                // float tempx = coord_x, tempy = coord_y;

                myMutex.lock();
                switch (currentdirect) {
                case 0:
                    coord_y -= speed;
                    break;
                case 1:
                    coord_y += speed;
                    break;
                case 2:
                    coord_x -= speed;
                    break;
                case 3:
                    coord_x += speed;
                    break;
                }
                myMutex.unlock();

                for (int j = 0; j < ebullets.size(); j++) {
                    if (ebullets[j].exist == true) {
                        myMutex.lock();
                        ebullets[j].x += ebullets[j].shot_speed * ebullets[j].dir_x;
                        ebullets[j].y += ebullets[j].shot_speed * ebullets[j].dir_y;
                        ebullets[j].pass += ebullets[j].shot_speed;

                        myMutex.unlock();

                        if (ebullets[j].pass > ebullets[j].range) {
                            ebullets[j].exist = false;
                        }
                    }
                }
                myMutex.lock();
                switch (id) {
                case 1:
                    switch (lor) {
                    case 0:
                        if (picture == 0)
                            enemy = "images/С��1������1.png";
                        else
                            enemy = "images/С��1������2.png";
                        break;
                    case 1:
                        if (picture == 0)
                            enemy = "images/С��1������1.png";
                        else
                            enemy = "images/С��1������2.png";
                    }
                    break;
                case 2:
                    switch (lor) {
                    case 0:
                        if (picture == 0)
                            enemy = "images/С��2������1.png";
                        else
                            enemy = "images/С��2������2.png";
                        break;
                    case 1:
                        if (picture == 0)
                            enemy = "images/С��2������1.png";
                        else
                            enemy = "images/С��2������2.png";
                    }
                    break;
                }
                myMutex.unlock();

                add1tion++;
                if (add1tion == 5) {
                    picture = (picture + 1) % 2;
                    add1tion = 0;
                }
                Sleep(20);
            }
        }

        //��Ӧ�����ж��յ��˺��ĺ���
    }
};

class Boss : public Enemy {
private:
    // 1.����Ļ5�� 2.׷�ٵ�Ļ10��3.����Ļ10��
    int type = 0;
    int count_b = 0;
    int count = 0;
    float bullet_delay = 0.5;
    float skill_delay = 2;
    bool isAttack = true;
    Hero& hero;

public:
    Boss(Hero&);
    Boss(int, int, Hero&);
    void bossMove();
    void restart();
};
Boss::Boss(Hero& hero_) : hero{ hero_ } {}
Boss::Boss(int tempx, int tempy, Hero& hero_) : hero{ hero_ } {
    coord_x = tempx;
    coord_y = tempy;
}
void Boss::restart() {
    exist = true;
    hp = 2000;
    ebullets.clear();
}
void Boss::bossMove() {

    sf::Mutex myMutex;
    Texture ebullet;
    Sprite bullet;
    ebullet.loadFromFile("images/bullet1.png");
    bullet.setTexture(ebullet);

    float distancex{ 0 };
    float distancey{ 0 };
    float directx{ 0 };
    float directy{ 0 };

    int allowdistance{ 0 };
    float distance{ 0 };

    int picture = 0;
    int add1tion{ 0 };

    std::mt19937 rng;                 // 1300,900
    rng.seed(std::random_device()()); //��ʼ���������
    std::uniform_int_distribution<std::mt19937::result_type> attacktype(1, 3);
    std::uniform_int_distribution<std::mt19937::result_type> randomdirect(0, 3);
    std::uniform_int_distribution<std::mt19937::result_type> ifattack(0, 10);
    std::uniform_int_distribution<std::mt19937::result_type> distance_(0, 1960);
    std::uniform_int_distribution<std::mt19937::result_type> dir{ 0, 1000 };
    sf::Clock clock_b, clock_s;
    float time_b = 0;
    float time_s = 0;
    while (1) {
        //����boss���������λ�ýǶ�
        time_b += clock_b.restart().asSeconds();
        time_s += clock_s.restart().asSeconds();
        distancex = hero.x - coord_x;
        distancey = hero.y - coord_y;
        directx = distancex / sqrt(distancex * distancex + distancey * distancey);
        directy = distancey / sqrt(distancex * distancex + distancey * distancey);

        distance = sqrt(distancex * distancex + distancey * distancey) / 4;

        //������ڼ�����ȴ���ͷ�
        if (time_s > skill_delay) {
            isAttack = true;
            type = attacktype(rng);
            if (type == 1) {
                count_b = 5;
            }
            else {
                count_b = 10;
            }
        }
        //������Թ���
        if (isAttack && time_b > bullet_delay) {
            switch (type) {
            case 1: //����
                ebullets.push_back(
                    eBullet(this->coord_x + 80, this->coord_y + 88, 0, -1)); //��
                ebullets.push_back(
                    eBullet(this->coord_x + 80, this->coord_y + 88, -1, 0)); //��
                ebullets.push_back(
                    eBullet(this->coord_x + 80, this->coord_y + 88, 0, 1)); //��
                ebullets.push_back(
                    eBullet(this->coord_x + 80, this->coord_y + 88, 1, 0)); //��

                break;
            case 2: //׷��
                ebullets.push_back(
                    eBullet(this->coord_x + 80, this->coord_y + 88, directx, directy));
                break;
            case 3: // 8��
                ebullets.push_back(
                    eBullet(this->coord_x + 80, this->coord_y + 88, 0, -1)); //��
                ebullets.push_back(
                    eBullet(this->coord_x + 80, this->coord_y + 88, -1, 0)); //��
                ebullets.push_back(
                    eBullet(this->coord_x + 80, this->coord_y + 88, 0, 1)); //��
                ebullets.push_back(
                    eBullet(this->coord_x + 80, this->coord_y + 88, 1, 0)); //��
                ebullets.push_back(eBullet(this->coord_x + 80, this->coord_y + 88,
                    -0.707, -0.707)); //����
                ebullets.push_back(eBullet(this->coord_x + 80, this->coord_y + 88,
                    -0.707, 0.707)); //����
                ebullets.push_back(eBullet(this->coord_x + 80, this->coord_y + 88,
                    0.707, 0.707)); //����
                ebullets.push_back(eBullet(this->coord_x + 80, this->coord_y + 88,
                    0.707, -0.707)); //����
            }
            time_b = 0; //�ӵ���ȴ

            //�����ӵ�����
            if (++count > count_b) {
                isAttack = false;
                time_s = 0;
                count = 0;
            }
        }
        myMutex.lock();
        //�������ƶ�
        coord_x += speed * directx;
        coord_y += speed * directy;

        //�ӵ�����
        for (int j = 0; j < ebullets.size(); j++) {
            if (ebullets[j].exist == true) {
                ebullets[j].x += ebullets[j].shot_speed * ebullets[j].dir_x;
                ebullets[j].y += ebullets[j].shot_speed * ebullets[j].dir_y;
                myMutex.unlock();

                if (ebullets[j].pass > ebullets[j].range) {
                    ebullets[j].exist = false;
                }
            }
        }
        myMutex.lock();
        //���������Ϸ�
        if (directx >= 0 && directy < 0) {
            if (picture == 0)
                enemy = "images/bossR1.png";
            else
                enemy = "images/bossR2.png";

        }
        //���������·�
        else if (directx > 0 && directy >= 0) {

            if (picture == 0)
                enemy = "images/boss2.png";
            else
                enemy = "images/boss3.png";
        }
        //���������Ϸ�
        else if (directx <= 0 && directy < 0) {
            if (picture == 0)
                enemy = "images/bossL1.png";
            else
                enemy = "images/bossL2.png";

        }
        //���������·�
        else if (directx < 0 && directy >= 0) {
            if (picture == 0)
                enemy = "images/boss2.png";
            else
                enemy = "images/boss3.png";
        }

        myMutex.unlock();

        add1tion++;
        if (add1tion == 5) {
            picture = (picture + 1) % 2;
            add1tion = 0;
        }
        Sleep(30);
    }
    //��Ӧ�����ж��յ��˺��ĺ���
}
