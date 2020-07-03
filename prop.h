#pragma once
#include "enemy.h"
#include "hero.h"
#include "shoot.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include <queue>
#include <random>
#include <string>
using std::string;
bool checkCollision(Sprite s1, Sprite s2) {
    return s1.getGlobalBounds().intersects(s2.getGlobalBounds());
}
// ��ײ��⺯��
// bool checkCollision(Sprite s1, Sprite s2) {
//	return s1.getGlobalBounds().intersects(s2.getGlobalBounds());
//}

//��������
class Counter {      //֡��������
    int now{ 0 };        //��ǰΪ����֡
    bool isRunning{ 0 }; //�Ƿ��ڼ���
    int limit{ 10 };     //����
public:
    Counter();
    Counter(int lim);
    int getNow();
    int getLimit();
    bool getRun();
    void add();             //������1
    void reset();           //��������0
    void shutdown();        //�رռ�����
    void setup();           //���ü�����
    void setLimit(int lim); //��������
};

Counter::Counter() { isRunning = 1; }
Counter::Counter(int lim) {
    isRunning = 1;
    limit = lim;
}
int Counter::getNow() { return now; }
int Counter::getLimit() { return limit; }
bool Counter::getRun() { return isRunning; }
void Counter::add() { //��ѭ������ĩβִ��
    if (isRunning) {
        if (now < limit)
            now++;
        else {
            reset();
            shutdown();
        }
    }
}
void Counter::reset() { //����������
    now = 0;
}
void Counter::shutdown() { //�رռ�����
    isRunning = 0;
}
void Counter::setup() { //����������
    isRunning = 1;
}
void Counter::setLimit(int lim) { //��������
    limit = lim;
}

//��
class Box { //����
    float x{ 0 };
    float y{ 0 };
    bool state{ 1 };     //δ��Ϊ1
    sf::Sprite cimage; //δ��
    sf::Sprite oimage; //�Ѵ�
public:
    Box() = default;
    Box(float x_, float y_);
    float getx();
    float gety();
    sf::Sprite getCimage();
    sf::Sprite getOimage();
    void detect(float hx, float hy);
    bool getState();
    void open(); //�򿪱���
    void drop(); //���ߵ���
};

class Prop { //������
protected:
    float x{ 0 };  //����x
    float y{ 0 };  //����y
    int mode{ 2 }; //�������� 1:fheal 2:pheal 3:bomb 4:lvup 5:pause
    sf::Sprite image;

public:
    Prop() = default;
    Prop(float x_, float y_, int mode_);
    sf::Sprite getImage();
    int getMode();
};

class Bomb : public Prop {
    bool isOver{ 0 };
    Counter counter;

public:
    Bomb(int lim, float x_, float y_);
    float getx();
    float gety();
    void setPos(int x_, int y_);
    void run();
    bool getOver();
};

class PropMenu { //�������ṹ��
public:
    int num{ 0 };
    bool isChosen{ 0 };
    void reset();
};

class Effect {
    float x;
    float y;
    float length{ 1 };
    float width{ 1 };
    int gap{ 1 };
    int state{ 0 };
    Counter counter{ 50 };
    bool isOver{ 0 };
    std::vector<sf::Sprite> images{};

public:
    Effect() = default;
    Effect(int lim, int g, float x, float y);
    Effect(int lim, int g, float x, float y, float l, float w);
    void addImage(sf::Sprite s);
    bool getOver();
    void run();
};

//����    *����δ�����Ҫ�޸�
void textureInit();                //��ʼ������ͼƬ����
void boxInit(int min, int max);    //��ʼ������λ��
void boxShow();                    //������
void propPick();                   //ʰȡ����
void propShow();                   //��ʾ�������
void propMenuShow();               //չʾ��Ʒ��*
void prop_to_master();             //ʹ�ö�������Ч�ĵ���
void prop_bomb(Vector2f mouse_xy); //ը����ʹ�ã���Ҫ�ŵ�����֧��
void prop_bomb_over();             //ը�����Ž�����Ҫը�� ---7.1
void prop_pause();                 //ʱ��ֹͣ��ʹ�úͽ��
void effectShow();                 //��Ч����

//������װ ���Է�װ���࣬�����Ҫ�Ļ�
void propsystem_init();      //����ϵͳ��ʼ�����ŵ�ѭ����֮ǰ
void propsystem_run_data();  //����ϵͳ����:�޸�����
void propsystem_run_fdraw(); //����ϵͳ���У��������ǡ��������
void propsystem_run_ldraw(); //����ϵͳ���У��������ǡ��������

//��������
//����extern�ı�����Ҫ�ŵ�main.cpp
extern Hero h1;                             //���ǵĶ���
extern Weapon b1;                           //�����Ķ���
extern int nEnemy;                          //����ʣ������
extern sf::Sprite s4;                       //���ǵ�sprite
extern std::vector<Enemy> enemy;            //���˵Ķ�������
extern std::vector<sf::Sprite> enemySprite; //����ͼ��Ķ�������
extern std::vector<Vector2f> barrier;       //�ϰ���
extern RenderWindow window;                 //����

extern std::list<Box> boxes; //δ����ı���,ͨ��ʱ��Ҫ���
extern std::list<Box>::iterator boxes_index; //�����б������
extern std::list<Prop> prop;                 //δʰȡ�ĵ���
extern std::list<Prop>::iterator prop_index; //�����б������
extern PropMenu propmenu[6]; //������ ������� ��Ҫpropmenu[0]��
extern std::vector<Counter> pauses; //ʱ��ֹͣ����ʱ
extern std::vector<Bomb> bombs;     //ը������ʱ����
extern std::vector<Effect> effects; //��Ч����

extern sf::Texture box_tex;      //��������
extern sf::Texture box_open_tex; //�򿪵ı�������
extern sf::Texture fheal_tex;
extern sf::Texture pheal_tex;
extern sf::Texture bomb_tex;
extern sf::Texture lvup_tex;
extern sf::Texture pause_tex;
extern sf::Texture proplist; //����������
extern sf::Texture bomb1, bomb2, bomb3;
extern sf::Texture open1, open2, open3, open4, open5, open6, open7, open8,
open9;
extern sf::Texture burst1, burst2, burst3, burst4, burst5, burst6, burst7,
burst8;
extern sf::Texture pheal1, pheal2, pheal3, pheal4, pheal5;
extern sf::Texture HPUP, TMP, LVUP;
extern SoundBuffer buffer1, buffer2, buffer3, buffer4, buffer5, buffer6,
buffer7;
extern Sound press, mdeath, bullet_t, bullet_f, mbullet, heal1, heal2, bomb_s, box_s;

/*
Hero h1;//���ǵĶ���
Weapon b1;//�����Ķ���
sf::Sprite s4;//���ǵ�sprite
std::vector<Enemy>enemy;//���˵Ķ�������
std::vector<sf::Sprite>enemySprite;//����ͼ��Ķ�������
std::vector<Vector2f> barrier; //�ϰ���
RenderWindow window;//����

std::list<Box> boxes; //δ����ı���,ͨ��ʱ��Ҫ���
std::list<Box>::iterator boxes_index;  //�����б������
std::list<Prop>prop; //δʰȡ�ĵ���
std::list<Prop>::iterator prop_index; //�����б������
PropMenu propmenu[6]; //������ ������� ��Ҫpropmenu[0]��
std::vector<Counter>pauses;//ʱ��ֹͣ����ʱ
std::queue<Bomb>bombs;//ը������ʱ����
std::vector<Effect>effects;//��Ч����
std::vector<Effect>::iterator effects_index;

sf::Texture box_tex; //��������
sf::Texture box_open_tex; //�򿪵ı�������
sf::Texture fheal_tex;
sf::Texture pheal_tex;
sf::Texture bomb_tex;
sf::Texture lvup_tex;
sf::Texture pause_tex;
sf::Texture proplist;//����������
sf::Texture bomb1, bomb2, bomb3;
sf::Texture open1, open2, open3, open4, open5, open6, open7, open8, open9;
sf::Texture burst1, burst2, burst3, burst4, burst5, burst6, burst7, burst8;
sf::Texture pheal1, pheal2, pheal3, pheal4, pheal5;
*/

//���Ա����
Box::Box(float x_, float y_) { // ctor
    x = x_;
    y = y_;
    cimage = Sprite{ box_tex };
    cimage.setScale(0.8f, 0.8f);
    cimage.setPosition(x_, y_);
    oimage = Sprite{ box_open_tex };
    oimage.setScale(0.8f, 0.8f);
    oimage.setPosition(x_, y_);
}
float Box::getx() { // getter
    return x;
}
float Box::gety() { // getter
    return y;
}
bool Box::getState() { // getter
    return state;
}
void Box::drop() { //�������
    std::mt19937 rng;
    rng.seed(std::random_device()()); //��ʼ���������
    std::uniform_int_distribution<std::mt19937::result_type> poss(1, 100);
    int temp = poss(rng);
    if (temp > 0 && temp <= 20)
        prop.push_back(Prop{ x, y, 1 });
    else if (temp > 20 && temp <= 40)
        prop.push_back(Prop{ x, y, 2 });
    else if (temp > 40 && temp <= 60)
        prop.push_back(Prop{ x, y, 3 });
    else if (temp > 60 && temp <= 80)
        prop.push_back(Prop{ x, y, 4 });
    else if (temp > 80 && temp <= 100)
        prop.push_back(Prop{ x, y, 5 });
}
sf::Sprite Box::getCimage() { // getter
    return cimage;
}
sf::Sprite Box::getOimage() { // getter
    return oimage;
}
void Box::open() { //���䱻��
  /* ����������Ҫ�ӵ�main������֧��
  case sk::F://��F������
          for (boxes_index = boxes.begin(); boxes_index != boxes.end();
  boxes_index++) { boxes_index->detect(h1.getCoord().x, h1.getCoord().y);
          }
          skills = 'F';
          break;
  */
    box_s.play();
    effects.push_back(Effect{ 18, 2, x - 55, y - 100 });
    effects.back().addImage(sf::Sprite{ open1 });
    effects.back().addImage(sf::Sprite{ open2 });
    effects.back().addImage(sf::Sprite{ open3 });
    effects.back().addImage(sf::Sprite{ open4 });
    effects.back().addImage(sf::Sprite{ open5 });
    effects.back().addImage(sf::Sprite{ open6 });
    effects.back().addImage(sf::Sprite{ open7 });
    effects.back().addImage(sf::Sprite{ open8 });
    effects.back().addImage(sf::Sprite{ open9 });
    state = 0;
}
void Box::detect(float hx, float hy) { //����������
    if (fabs(hx - x) < 150 && fabs(hy - y) < 150 && state) {
        drop();
        open();
    }
}

Prop::Prop(float x_, float y_, int mode_) { // ctor
    x = x_;
    y = y_;
    mode = mode_;
    if (mode_ == 1)
        image = Sprite{ fheal_tex };
    else if (mode_ == 2)
        image = Sprite{ pheal_tex };
    else if (mode_ == 3)
        image = Sprite{ bomb_tex };
    else if (mode_ == 4)
        image = Sprite{ lvup_tex };
    else if (mode_ == 5)
        image = Sprite{ pause_tex };
    image.setPosition(x_, y_);
    image.setScale(0.2, 0.2);
}
sf::Sprite Prop::getImage() { // getter
    return image;
}
int Prop::getMode() { // getter
    return mode;
}

Bomb::Bomb(int lim, float x_, float y_) { // ctor
    x = x_;
    y = y_;
    counter.setLimit(lim);
    mode = 3;
}
float Bomb::getx() { // getter
    return x;
}
float Bomb::gety() { // getter
    return y;
}
bool Bomb::getOver() { return isOver; }
void Bomb::run() {
    counter.add();
    if (!counter.getRun())
        isOver = 1;
}
void Bomb::setPos(int x_, int y_) { // setter
    image.setPosition(x_, y_);
}

void PropMenu::reset() { //����ѡ��״̬
    isChosen = 0;
}

Effect::Effect(int lim, int g, float x, float y) {
    this->x = x;
    this->y = y;
    gap = g;
    counter.setLimit(lim);
}
Effect::Effect(int lim, int g, float x, float y, float l, float w) {
    this->x = x;
    this->y = y;
    length = l;
    width = w;
    gap = g;
    counter.setLimit(lim);
}
bool Effect::getOver() { return isOver; }
void Effect::addImage(sf::Sprite s) {
    images.push_back(s);
    images.back().setPosition(x, y);
    images.back().setScale(length, width);
}
void Effect::run() {
    if (isOver == 0) {
        counter.add();
        if (state < images.size())
            window.draw(images[state]);
        if (counter.getNow() % gap == 0 && state < images.size())
            state++;
        else if (state == images.size()) {
            state = 0;
            isOver = 1;
        }
        if (!counter.getRun())
            isOver = 1;
    }
}

//����
void textureInit() { //��ʼ���������ֵ����õ�ͼƬ
    box_tex.loadFromFile("images/box.png");
    box_open_tex.loadFromFile("images/box_open.png");
    fheal_tex.loadFromFile("images/fheal.png");
    pheal_tex.loadFromFile("images/pheal.png");
    bomb_tex.loadFromFile("images/bomb.png");
    lvup_tex.loadFromFile("images/lvup.png");
    pause_tex.loadFromFile("images/pause.png");
    proplist.loadFromFile("images/proplist.png");
    burst1.loadFromFile("images/burst1.png");
    burst2.loadFromFile("images/burst2.png");
    burst3.loadFromFile("images/burst3.png");
    burst4.loadFromFile("images/burst4.png");
    burst5.loadFromFile("images/burst5.png");
    burst6.loadFromFile("images/burst6.png");
    burst7.loadFromFile("images/burst7.png");
    burst8.loadFromFile("images/burst8.png");
    bomb1.loadFromFile("images/bomb1.png");
    bomb2.loadFromFile("images/bomb2.png");
    bomb3.loadFromFile("images/bomb3.png");
    open1.loadFromFile("images/open1.png");
    open2.loadFromFile("images/open2.png");
    open3.loadFromFile("images/open1.png");
    open4.loadFromFile("images/open4.png");
    open5.loadFromFile("images/open5.png");
    open6.loadFromFile("images/open6.png");
    open7.loadFromFile("images/open7.png");
    open8.loadFromFile("images/open8.png");
    open9.loadFromFile("images/open9.png");
    pheal1.loadFromFile("images/pheal1.png");
    pheal2.loadFromFile("images/pheal2.png");
    pheal3.loadFromFile("images/pheal3.png");
    pheal4.loadFromFile("images/pheal4.png");
    pheal5.loadFromFile("images/pheal5.png");
    HPUP.loadFromFile("images/HPUP.png");
    LVUP.loadFromFile("images/LV.png");
    TMP.loadFromFile("images/TMP.png");
}

void boxInit(int min, int max) { //�������1-3������
    std::mt19937 rng;
    rng.seed(std::random_device()()); //��ʼ���������
    std::uniform_int_distribution<std::mt19937::result_type> num_range(min, max);
    int num = num_range(rng); //������ɱ�����

    while (num-- > 0) {
        std::mt19937 rng;
        rng.seed(std::random_device()()); //��ʼ���������
        std::uniform_int_distribution<std::mt19937::result_type> x_(
            300, 1100); //��Χ���޸�
        std::uniform_int_distribution<std::mt19937::result_type> y_(
            300, 650); //��Χ���޸�
        float temp_x = x_(rng);
        float temp_y = y_(rng); //��ʼ�����λ��
        int temp_flag = 1;

        sf::Sprite temp_s1{ box_tex };
        temp_s1.setScale(0.8, 0.8);
        sf::Texture block;
        block.loadFromFile("images/block.png");
        sf::Sprite temp_s2{ block };
        temp_s2.setScale(0.5, 0.5);

        bool flag = 1;
        temp_s1.setPosition(temp_x, temp_y);

        for (int i = 0; i < barrier.size(); i++) { //��ⱦ�������Ƿ���ϰ����غ�
            temp_s2.setPosition(barrier[i].x, barrier[i].y);
            if (checkCollision(temp_s1, temp_s2)) {
                flag = 0;
                break;
            }
        }

        for (boxes_index = boxes.begin(); boxes_index != boxes.end();
            boxes_index++) { //������ɵı����Ƿ��غ�
            if (checkCollision(temp_s1, boxes_index->getCimage())) {
                flag = 0;
                break;
            }
        }

        if (flag)
            boxes.push_back(Box{ temp_x, temp_y });
    }
}

void boxShow() { //�ڵ�ͼ�ϻ�����
    for (boxes_index = boxes.begin(); boxes_index != boxes.end(); boxes_index++) {
        if (boxes_index->getState() == 1) //δ�򿨵ı���
            window.draw(boxes_index->getCimage());
        else if (boxes_index->getState() == 0) //�Ѵ򿪵ı���
            window.draw(boxes_index->getOimage());
    }
}

void propShow() { //��ʾ�������
    for (prop_index = prop.begin(); prop_index != prop.end(); prop_index++)
        window.draw(prop_index->getImage());
}

void propMenuShow() { //չʾ��Ʒ��*
    sf::Sprite proplist_{ proplist };
    proplist_.setScale(0.3f, 0.3f);
    proplist_.setPosition(300, 800);
    window.draw(proplist_); //����Ʒ��
}

void propPick() { //�������Ƿ�ʰȡ����ʰȡ��propmenu��Ӧ������Ŀ��1
    for (prop_index = prop.begin(); prop_index != prop.end();) {
        if (checkCollision(prop_index->getImage(), s4)) { //������Ǻ͵�����ײ
            switch (prop_index->getMode()) {                //������Ŀ����
            case 1:
                propmenu[1].num++;
                break;
            case 2:
                propmenu[2].num++;
                break;
            case 3:
                propmenu[3].num++;
                break;
            case 4:
                propmenu[4].num++;
                break;
            case 5:
                propmenu[5].num++;
                break;
            }
            prop.erase(prop_index++); //�ӵ�������б��Ƴ�
            continue;
        }
        prop_index++;
    }
}

void prop_to_master() {      //ʹ�ö�������Ч�ĵ���
    int health = h1.getHp();   //��������ֵ
    int wplv = b1.getDamage(); //�����ȼ�
    if (propmenu[1].isChosen) {
        if (propmenu[1].num) {
            h1.setHp(100); //��Ѫƿ������
            propmenu[1].num--;
            propmenu[1].reset();
            effects.push_back(
                Effect{ 5, 1, h1.getCoord().x - 20, h1.getCoord().y - 100 });
            effects.back().addImage(Sprite{ pheal1 });
            effects.back().addImage(Sprite{ pheal2 });
            effects.back().addImage(Sprite{ pheal3 });
            effects.back().addImage(Sprite{ pheal4 });
            effects.back().addImage(Sprite{ pheal5 });
            effects.push_back(Effect{15,15,1200, 200,0.2,0.2 });
            effects.back().addImage(Sprite{ HPUP });
            heal1.play();
        }
        else
            propmenu[1].reset();
    }

    else if (propmenu[2].isChosen) { //СѪƿ��2
        if (propmenu[2].num) {
            if (health <= 100 - 20)
                h1.setHp(health + 20);
            else
                h1.setHp(100);
            propmenu[2].num--;
            propmenu[2].reset();
            effects.push_back(
                Effect{ 5, 1, h1.getCoord().x - 20, h1.getCoord().y - 100 });
            effects.back().addImage(Sprite{ pheal1 });
            effects.back().addImage(Sprite{ pheal2 });
            effects.back().addImage(Sprite{ pheal3 });
            effects.back().addImage(Sprite{ pheal4 });
            effects.back().addImage(Sprite{ pheal5 });
            effects.push_back(Effect{ 15,15,1200, 200,0.2,0.2 });
            effects.back().addImage(Sprite{ HPUP });
            heal2.play();
        }
        else
            propmenu[2].reset();
    }

    else if (propmenu[4].isChosen) { //������һ��
        if (propmenu[4].num) {
            if (wplv < 3)
                b1.setDamage(wplv + 1);
            else
                b1.setDamage(3);
            propmenu[4].num--;
            propmenu[4].reset();
            effects.push_back(Effect{ 15,15,1200, 200,0.2,0.2 });
            effects.back().addImage(Sprite{ LVUP });
        }
        else
            propmenu[4].reset();
    }
}

void prop_bomb(sf::Vector2f mouse_xy) { //ը����ʹ�ã���Ҫ�ŵ�����֧��
    if (propmenu[3].isChosen && propmenu[3].num) {
        float tempsize = 250 * 0.2;
        effects.push_back(
            Effect{ 9, 3, mouse_xy.x - tempsize, mouse_xy.y - tempsize, 0.2, 0.2 });
        effects.back().addImage(Sprite{ bomb1 });
        effects.back().addImage(Sprite{ bomb2 });
        effects.back().addImage(Sprite{ bomb3 });
        bombs.push_back(Bomb{ 9, mouse_xy.x, mouse_xy.y });
        propmenu[3].num--;
        propmenu[3].reset();
    }
    else if (propmenu[3].isChosen && !propmenu[3].num)
        propmenu[3].reset();
}

void prop_bomb_over() { //ը�����Ž�����Ҫը��

    for (int i = 0; i < bombs.size(); i++) //ը����ʱ������ʱ
        bombs[i].run();
    if (!bombs.empty()) {
        if (bombs.back().getOver()) { //ը�����ŵ�ʱ��
            bomb_s.play();
            for (int i = 0; i < enemy.size(); i++) { //ը�����Է�Χ�ڵ�С�����˺�//no
                int dx = abs(enemySprite[i].getPosition().x - bombs.back().getx()); //
                int dy = abs(enemySprite[i].getPosition().y - bombs.back().gety());
                if (dx <= 500 || dy <= 500)
                    enemy[i].beHitted(100); //ը���˺�
            }
            effects.push_back(Effect{ 16, 2, bombs.back().getx() - 350,
                                     bombs.back().gety() - 350, 1, 1 });
            effects.back().addImage(Sprite{ burst1 });
            effects.back().addImage(Sprite{ burst2 });
            effects.back().addImage(Sprite{ burst3 });
            effects.back().addImage(Sprite{ burst4 });
            effects.back().addImage(Sprite{ burst5 });
            effects.back().addImage(Sprite{ burst6 });
            effects.back().addImage(Sprite{ burst7 });
            effects.back().addImage(Sprite{ burst8 });
            bombs.pop_back();
        }
    }
}

void effectShow() { //������Ч
    for (int i = 0; i < effects.size(); i++) {
        if (!effects[i].getOver()) {
            effects[i].run();
        }
    }
}

void prop_pause() { //ʱ��ֹͣ��Ч�ͽ��
    if (propmenu[5].isChosen &&
        propmenu[5].num) { //ʱ��ֹͣ������С��ֹͣ�ƶ�һ��ʱ��
        pauses.push_back(Counter{ 100 }); //ʱ��ֹͣ��֡��
        for (int i = 0; i < enemy.size(); i++) {
            enemy[i].setSpeed(0);
            std::cout << enemy[i].speed << std::endl;
        }
        propmenu[5].num--;
        propmenu[5].reset();
        effects.push_back(Effect{ 15,15,1200, 200,0.2,0.2 });
        effects.back().addImage(Sprite{ TMP });
    }
    else if (propmenu[5].isChosen && !propmenu[5].num)
        propmenu[5].reset();

    if (!pauses.empty()) {
        for (int i = 0; i < pauses.size(); i++)
            pauses[i].add();
    }

    if (!pauses.empty()) {
        if (!pauses.back().getRun()) { //ʱ��ֹͣ����
            for (int i = 0; i < enemy.size(); i++)
                enemy[i].setSpeed(3);
            pauses.pop_back();
        }
    }
}

//�����Ҫ���Է�װ������

// void boxInit(int min, int max) �������ɺ���������Ϊ������Χ

void propsystem_init() { //����ϵͳ��ʼ��,һ����Ϸֻ��Ҫ����һ��
    textureInit();         //��ʼ������ͼƬ����
}

void propsystem_run_data() { //����ϵͳ���У��޸�����
    propPick();                //�������Ƿ�ʰȡ
    prop_to_master();          //ʹ�ö�������Ч�ĵ���
    prop_bomb_over();          //ը�����Ž�����Ҫը�� ---7.1
    prop_pause();              //ʱ��ֹͣ��ʹ�úͽ��
}

//���º����ŵ�print_back()����
void propsystem_run_fdraw() { //����ϵͳ���У��������ǡ��������
    boxShow();                  //������
    propShow();                 //��ʾ�������
}

void propsystem_run_ldraw() { //����ϵͳ���У��������ǡ��������
    effectShow();               //��Ч����
    propMenuShow();             //չʾ��Ʒ��
}
