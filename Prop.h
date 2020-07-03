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
// 碰撞检测函数
// bool checkCollision(Sprite s1, Sprite s2) {
//	return s1.getGlobalBounds().intersects(s2.getGlobalBounds());
//}

//计数器类
class Counter {      //帧数计数器
    int now{ 0 };        //当前为多少帧
    bool isRunning{ 0 }; //是否在计数
    int limit{ 10 };     //上限
public:
    Counter();
    Counter(int lim);
    int getNow();
    int getLimit();
    bool getRun();
    void add();             //计数加1
    void reset();           //计数器归0
    void shutdown();        //关闭计数器
    void setup();           //启用计数器
    void setLimit(int lim); //重置上限
};

Counter::Counter() { isRunning = 1; }
Counter::Counter(int lim) {
    isRunning = 1;
    limit = lim;
}
int Counter::getNow() { return now; }
int Counter::getLimit() { return limit; }
bool Counter::getRun() { return isRunning; }
void Counter::add() { //在循环体最末尾执行
    if (isRunning) {
        if (now < limit)
            now++;
        else {
            reset();
            shutdown();
        }
    }
}
void Counter::reset() { //计数器清零
    now = 0;
}
void Counter::shutdown() { //关闭计数器
    isRunning = 0;
}
void Counter::setup() { //启动计数器
    isRunning = 1;
}
void Counter::setLimit(int lim) { //重置上限
    limit = lim;
}

//类
class Box { //宝箱
    float x{ 0 };
    float y{ 0 };
    bool state{ 1 };     //未打卡为1
    sf::Sprite cimage; //未打开
    sf::Sprite oimage; //已打开
public:
    Box() = default;
    Box(float x_, float y_);
    float getx();
    float gety();
    sf::Sprite getCimage();
    sf::Sprite getOimage();
    void detect(float hx, float hy);
    bool getState();
    void open(); //打开宝箱
    void drop(); //道具掉落
};

class Prop { //道具类
protected:
    float x{ 0 };  //坐标x
    float y{ 0 };  //坐标y
    int mode{ 2 }; //道具种类 1:fheal 2:pheal 3:bomb 4:lvup 5:pause
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

class PropMenu { //道具栏结构体
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

//函数    *代表未完成需要修改
void textureInit();                //初始化所有图片纹理
void boxInit(int min, int max);    //初始化宝箱位置
void boxShow();                    //画宝箱
void propPick();                   //拾取道具
void propShow();                   //显示掉落道具
void propMenuShow();               //展示物品栏*
void prop_to_master();             //使用对主角生效的道具
void prop_bomb(Vector2f mouse_xy); //炸弹的使用，需要放到鼠标分支里
void prop_bomb_over();             //炸弹引信结束，要炸了 ---7.1
void prop_pause();                 //时间停止的使用和解除
void effectShow();                 //特效绘制

//函数封装 可以封装成类，如果需要的话
void propsystem_init();      //道具系统初始化，放到循环体之前
void propsystem_run_data();  //道具系统运行:修改数据
void propsystem_run_fdraw(); //道具系统运行：先于主角、怪物绘制
void propsystem_run_ldraw(); //道具系统运行：后于主角、怪物绘制

//变量引入
//所有extern的变量都要放到main.cpp
extern Hero h1;                             //主角的对象
extern Weapon b1;                           //武器的对象
extern int nEnemy;                          //怪物剩余数量
extern sf::Sprite s4;                       //主角的sprite
extern std::vector<Enemy> enemy;            //敌人的对象数组
extern std::vector<sf::Sprite> enemySprite; //敌人图像的对象数组
extern std::vector<Vector2f> barrier;       //障碍物
extern RenderWindow window;                 //窗口

extern std::list<Box> boxes; //未击碎的宝箱,通关时需要清空
extern std::list<Box>::iterator boxes_index; //宝箱列表迭代器
extern std::list<Prop> prop;                 //未拾取的道具
extern std::list<Prop>::iterator prop_index; //道具列表迭代器
extern PropMenu propmenu[6]; //道具栏 五个道具 不要propmenu[0]了
extern std::vector<Counter> pauses; //时间停止倒计时
extern std::vector<Bomb> bombs;     //炸弹倒计时队列
extern std::vector<Effect> effects; //特效数组

extern sf::Texture box_tex;      //宝箱纹理
extern sf::Texture box_open_tex; //打开的宝箱纹理
extern sf::Texture fheal_tex;
extern sf::Texture pheal_tex;
extern sf::Texture bomb_tex;
extern sf::Texture lvup_tex;
extern sf::Texture pause_tex;
extern sf::Texture proplist; //道具栏纹理
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
Hero h1;//主角的对象
Weapon b1;//武器的对象
sf::Sprite s4;//主角的sprite
std::vector<Enemy>enemy;//敌人的对象数组
std::vector<sf::Sprite>enemySprite;//敌人图像的对象数组
std::vector<Vector2f> barrier; //障碍物
RenderWindow window;//窗口

std::list<Box> boxes; //未击碎的宝箱,通关时需要清空
std::list<Box>::iterator boxes_index;  //宝箱列表迭代器
std::list<Prop>prop; //未拾取的道具
std::list<Prop>::iterator prop_index; //道具列表迭代器
PropMenu propmenu[6]; //道具栏 五个道具 不要propmenu[0]了
std::vector<Counter>pauses;//时间停止倒计时
std::queue<Bomb>bombs;//炸弹倒计时队列
std::vector<Effect>effects;//特效数组
std::vector<Effect>::iterator effects_index;

sf::Texture box_tex; //宝箱纹理
sf::Texture box_open_tex; //打开的宝箱纹理
sf::Texture fheal_tex;
sf::Texture pheal_tex;
sf::Texture bomb_tex;
sf::Texture lvup_tex;
sf::Texture pause_tex;
sf::Texture proplist;//道具栏纹理
sf::Texture bomb1, bomb2, bomb3;
sf::Texture open1, open2, open3, open4, open5, open6, open7, open8, open9;
sf::Texture burst1, burst2, burst3, burst4, burst5, burst6, burst7, burst8;
sf::Texture pheal1, pheal2, pheal3, pheal4, pheal5;
*/

//类成员函数
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
void Box::drop() { //掉落道具
    std::mt19937 rng;
    rng.seed(std::random_device()()); //初始化随机种子
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
void Box::open() { //宝箱被打开
  /* 以下内容需要加到main的鼠标分支中
  case sk::F://按F开宝箱
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
void Box::detect(float hx, float hy) { //减少运算量
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

void PropMenu::reset() { //重置选中状态
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

//函数
void textureInit() { //初始化纹理，各种道具用的图片
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

void boxInit(int min, int max) { //随机生成1-3个宝箱
    std::mt19937 rng;
    rng.seed(std::random_device()()); //初始化随机种子
    std::uniform_int_distribution<std::mt19937::result_type> num_range(min, max);
    int num = num_range(rng); //随机生成宝箱数

    while (num-- > 0) {
        std::mt19937 rng;
        rng.seed(std::random_device()()); //初始化随机种子
        std::uniform_int_distribution<std::mt19937::result_type> x_(
            300, 1100); //范围可修改
        std::uniform_int_distribution<std::mt19937::result_type> y_(
            300, 650); //范围可修改
        float temp_x = x_(rng);
        float temp_y = y_(rng); //初始化随机位置
        int temp_flag = 1;

        sf::Sprite temp_s1{ box_tex };
        temp_s1.setScale(0.8, 0.8);
        sf::Texture block;
        block.loadFromFile("images/block.png");
        sf::Sprite temp_s2{ block };
        temp_s2.setScale(0.5, 0.5);

        bool flag = 1;
        temp_s1.setPosition(temp_x, temp_y);

        for (int i = 0; i < barrier.size(); i++) { //检测宝箱坐标是否和障碍物重合
            temp_s2.setPosition(barrier[i].x, barrier[i].y);
            if (checkCollision(temp_s1, temp_s2)) {
                flag = 0;
                break;
            }
        }

        for (boxes_index = boxes.begin(); boxes_index != boxes.end();
            boxes_index++) { //检测生成的宝箱是否重合
            if (checkCollision(temp_s1, boxes_index->getCimage())) {
                flag = 0;
                break;
            }
        }

        if (flag)
            boxes.push_back(Box{ temp_x, temp_y });
    }
}

void boxShow() { //在地图上画宝箱
    for (boxes_index = boxes.begin(); boxes_index != boxes.end(); boxes_index++) {
        if (boxes_index->getState() == 1) //未打卡的宝箱
            window.draw(boxes_index->getCimage());
        else if (boxes_index->getState() == 0) //已打开的宝箱
            window.draw(boxes_index->getOimage());
    }
}

void propShow() { //显示掉落道具
    for (prop_index = prop.begin(); prop_index != prop.end(); prop_index++)
        window.draw(prop_index->getImage());
}

void propMenuShow() { //展示物品栏*
    sf::Sprite proplist_{ proplist };
    proplist_.setScale(0.3f, 0.3f);
    proplist_.setPosition(300, 800);
    window.draw(proplist_); //画物品栏
}

void propPick() { //检测道具是否被拾取，若拾取则propmenu对应道具数目加1
    for (prop_index = prop.begin(); prop_index != prop.end();) {
        if (checkCollision(prop_index->getImage(), s4)) { //检测主角和道具碰撞
            switch (prop_index->getMode()) {                //道具数目增加
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
            prop.erase(prop_index++); //从掉落道具列表移除
            continue;
        }
        prop_index++;
    }
}

void prop_to_master() {      //使用对主角生效的道具
    int health = h1.getHp();   //主角生命值
    int wplv = b1.getDamage(); //武器等级
    if (propmenu[1].isChosen) {
        if (propmenu[1].num) {
            h1.setHp(100); //大血瓶，回满
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

    else if (propmenu[2].isChosen) { //小血瓶回2
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

    else if (propmenu[4].isChosen) { //武器升一级
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

void prop_bomb(sf::Vector2f mouse_xy) { //炸弹的使用，需要放到鼠标分支里
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

void prop_bomb_over() { //炸弹引信结束，要炸了

    for (int i = 0; i < bombs.size(); i++) //炸弹计时器倒计时
        bombs[i].run();
    if (!bombs.empty()) {
        if (bombs.back().getOver()) { //炸弹引信到时间
            bomb_s.play();
            for (int i = 0; i < enemy.size(); i++) { //炸弹，对范围内的小怪有伤害//no
                int dx = abs(enemySprite[i].getPosition().x - bombs.back().getx()); //
                int dy = abs(enemySprite[i].getPosition().y - bombs.back().gety());
                if (dx <= 500 || dy <= 500)
                    enemy[i].beHitted(100); //炸弹伤害
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

void effectShow() { //道具特效
    for (int i = 0; i < effects.size(); i++) {
        if (!effects[i].getOver()) {
            effects[i].run();
        }
    }
}

void prop_pause() { //时间停止生效和解除
    if (propmenu[5].isChosen &&
        propmenu[5].num) { //时间停止，所有小怪停止移动一段时间
        pauses.push_back(Counter{ 100 }); //时间停止的帧数
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
        if (!pauses.back().getRun()) { //时间停止结束
            for (int i = 0; i < enemy.size(); i++)
                enemy[i].setSpeed(3);
            pauses.pop_back();
        }
    }
}

//如果需要可以封装到类中

// void boxInit(int min, int max) 宝箱生成函数，参数为数量范围

void propsystem_init() { //道具系统初始化,一次游戏只需要运行一次
    textureInit();         //初始化所有图片纹理
}

void propsystem_run_data() { //道具系统运行：修改数据
    propPick();                //检测道具是否被拾取
    prop_to_master();          //使用对主角生效的道具
    prop_bomb_over();          //炸弹引信结束，要炸了 ---7.1
    prop_pause();              //时间停止的使用和解除
}

//以下函数放到print_back()后面
void propsystem_run_fdraw() { //道具系统运行：先于主角、怪物绘制
    boxShow();                  //画宝箱
    propShow();                 //显示掉落道具
}

void propsystem_run_ldraw() { //道具系统运行：后于主角、怪物绘制
    effectShow();               //特效绘制
    propMenuShow();             //展示物品栏
}
