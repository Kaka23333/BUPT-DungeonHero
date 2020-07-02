#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include <string>
#include <random>
#include <queue>
#include"hero.h"
#include"enemy.h"
#include"shoot.h"
using std::string;

// 碰撞检测函数
bool checkCollision(Sprite s1, Sprite s2);
//{
	//return s1.getLocalBounds().intersects(s2.getLocalBounds());
//}


//计数器类
class Counter {//帧数计数器
	static int num;
	int no_;//计数器编号
	int now{ 0 };//当前为多少帧
	bool isOver{ 0 };//是否达到上限
	bool isRunning{ 0 };//是否在计数
	int limit{ 10 };//上限
public:
	Counter();
	Counter(int lim);
	int getNow();
	int getLimit();
	int getNo_();
	bool getOver();
	bool getRun();
	void add();//计数加1
	void reset();//计数器归0
	void shutdown();//关闭计数器
	void setup();//启用计数器
	void setLimit(int lim);//重置上限
};

Counter::Counter() {
	num++;
	no_ = num;
	isRunning = 1;
}
Counter::Counter(int lim) {
	num++;
	no_ = num;
	isRunning = 1;
	limit = lim;
}
int Counter::getNow() {
	return now;
}
int Counter::getLimit() {
	return limit;
}
bool Counter::getOver() {
	return isOver;
}
bool Counter::getRun() {
	return isRunning;
}
int Counter::getNo_() {
	return no_;
}
void Counter::add() {//在循环体最末尾执行
	if (isRunning) {
		if (now < limit)now++;
		else reset();
	}
}
void Counter::reset() {
	now = 0;
}
void Counter::shutdown() {
	isRunning = 0;
}
void Counter::setup() {
	isRunning = 1;
}
void Counter::setLimit(int lim) {//重置上限
	limit = lim;
}


//类
class Box {//宝箱
	float x{ 0 };
	float y{ 0 };
	bool state{ 1 };//未打卡为1
	sf::Sprite cimage;//未打开
	sf::Sprite oimage;//已打开
public:
	Box() = default;
	Box(float x_, float y_);
	float getx();
	float gety();
	sf::Sprite getCimage();
	sf::Sprite getOimage();
	void detect(float hx, float hy);
	bool getState();
	void open();//打开宝箱
	void drop();//道具掉落
};


class Prop {//道具类
protected:
	float x{ 0 }; //坐标x
	float y{ 0 }; //坐标y
	int mode{2}; //道具种类 1:fheal 2:pheal 3:bomb 4:lvup 5:pause
	sf::Sprite image;
public:
	Prop() = default;
	Prop(float x_, float y_, int mode_);
	sf::Sprite getImage();
	int getMode();
};


class Bomb : public Prop {
	bool isOver;
	Counter counter;
public:
	Bomb(int lim, float x_, float y_);
	float getx();
	float gety();
	void setPos(int x_, int y_);
	void run();
	bool getOver();
};


class PropMenu {//道具栏结构体
public:
	int num{ 0 };
	bool isChosen{ 0 };
	void reset();
};


class Effect {
	float x;
	float y;
	float length;
	float width;
	int gap{ 1 };
	int state{ 0 };
	Counter counter{ 50 };
	bool isOver{ 0 };
	std::vector<sf::Sprite>images{};
public:
	Effect() = default;
	Effect(int lim, int g, float x, float y);
	void addImage(sf::Texture t);
	bool getOver();
	void run();
};


//函数    *代表未完成需要修改
void textureInit();//初始化所有图片纹理
void boxInit();//初始化宝箱位置
void boxShow();//画宝箱
void propShow(); //显示掉落道具
void propMenuShow();//展示物品栏*
void prop_to_master();//使用对主角生效的道具
void prop_bomb(Vector2f mouse_xy);//炸弹的使用，需要放到鼠标分支里
void prop_bomb_over();//炸弹引信结束，要炸了 ---7.1
void prop_pause();//时间停止的使用和解除
void effectShow();//特效绘制


//函数封装 可以封装成类，如果需要的话
void prop_system_init();//道具系统初始化，放到循环体之前
void prop_system_run_draw();//道具系统运行:绘图
void prop_system_run_data();//道具系统运行:修改数据



//变量引入
//所有extern的变量都要放到main.cpp
extern Hero h1;//主角的对象
extern Weapon b1;//武器的对象
extern sf::Sprite s4;//主角的sprite
extern std::vector<Enemy>enemy;//敌人的对象数组
extern std::vector<sf::Sprite>enemySprite;//敌人图像的对象数组
extern std::vector<Vector2f> barrier; //障碍物
extern RenderWindow window;//窗口

extern std::list<Box> boxes; //未击碎的宝箱,通关时需要清空
extern std::list<Box>::iterator boxes_index;  //宝箱列表迭代器
extern std::list<Prop>prop; //未拾取的道具
extern std::list<Prop>::iterator prop_index; //道具列表迭代器
extern PropMenu propmenu[6]; //道具栏 五个道具 不要propmenu[0]了
extern std::vector<Counter>pauses;//时间停止倒计时
extern std::queue<Bomb>bombs;//炸弹倒计时队列
extern std::vector<Effect>effects;//特效数组
extern std::vector<Effect>::iterator effects_index;

extern sf::Texture box_tex; //宝箱纹理
extern sf::Texture box_open_tex; //打开的宝箱纹理
extern sf::Texture fheal_tex;
extern sf::Texture pheal_tex;
extern sf::Texture bomb_tex;
extern sf::Texture lvup_tex;
extern sf::Texture pause_tex;
extern sf::Texture proplist;//道具栏纹理




//类成员函数
Box::Box(float x_, float y_) {//ctor
	x = x_;
	y = y_;
	cimage = Sprite{ box_tex };
	cimage.setScale(0.8f, 0.8f);
	cimage.setPosition(x_, y_);
	oimage = Sprite{ box_open_tex };
	oimage.setScale(0.8f, 0.8f);
	oimage.setPosition(x_, y_);
}
float Box::getx() {//getter
	return x;
}
float Box::gety() {//getter
	return y;
}
bool Box::getState() {//getter
	return state;
}
void Box::drop() {//掉落道具
	std::mt19937 rng;
	rng.seed(std::random_device()());//初始化随机种子
	std::uniform_int_distribution<std::mt19937::result_type> poss(1, 100);
	int temp = poss(rng);
	if (temp > 0 && temp <= 20) prop.push_back(Prop{ x, y ,1 });
	else if (temp > 20 && temp <= 40)prop.push_back(Prop{ x, y, 2 });
	else if (temp > 40 && temp <= 60)prop.push_back(Prop{ x, y, 3 });
	else if (temp > 60 && temp <= 80)prop.push_back(Prop{ x, y ,4 });
	else if (temp > 80 && temp <= 100)prop.push_back(Prop{ x, y ,5 });
}
sf::Sprite Box::getCimage() {//getter
	return cimage;
}
sf::Sprite Box::getOimage() {//getter
	return oimage;
}
void Box::open() {//宝箱被打开
	/* 以下内容需要加到main的鼠标分支中
	case sk::F://按F开宝箱
		for (boxes_index = boxes.begin(); boxes_index != boxes.end(); boxes_index++) {
			boxes_index->detect(h1.getCoord().x, h1.getCoord().y);
		}
		skills = 'F';
		break;
	*/
	state = 0;
}
void Box::detect(float hx, float hy) {//getter//减少运算量
	if (fabs(hx - x) < 20 && fabs(hy - y) < 20 && state) {
		drop();
		open();
	}
}

Prop::Prop(float x_, float y_, int mode_) {//ctor
	x = x_;
	y = y_;
	mode = mode_;
	if (mode_ == 1)image = Sprite{ fheal_tex };
	else if (mode_ == 2)image = Sprite{ pheal_tex };
	else if (mode_ == 3)image = Sprite{ bomb_tex };
	else if (mode_ == 4)image = Sprite{ lvup_tex };
	else if (mode_ == 5)image = Sprite{ pause_tex };
	image.setScale(0.2f, 0.2f);
}
sf::Sprite Prop::getImage() {//getter
	return image;
}
int Prop::getMode() {//getter
	return mode;
}

Bomb::Bomb(int lim, float x_, float y_) {//ctor
	x = x_;
	y = y_;
	counter.setLimit(lim);
	mode = 3;
}
float Bomb::getx() {//getter
	return x;
}
float Bomb::gety() {//getter
	return y;
}
bool Bomb::getOver() {
	return isOver;
}
void Bomb::run() {
	counter.add();
	if (counter.getOver())isOver = 1;
}
void Bomb::setPos(int x_, int y_) {//setter
	image.setPosition(x_, y_);
}

void PropMenu::reset() {//重置选中状态
	isChosen = 0;
}

Effect::Effect(int lim, int g, float x, float y) {
	this->x = x;
	this->y = y;
	gap = g;
	counter.setLimit(lim);
}
bool Effect::getOver() {
	return isOver;
}
void Effect::addImage(sf::Texture t) {
	images.push_back(sf::Sprite{ t });
}
void Effect::run() {
	if (!counter.getOver()) {
		if (images.empty());
		else {
			if (counter.getNow() % gap == 0 && state < images.size()) {
				window.draw(images[state]);
				state++;
			}
			else if (counter.getNow() % gap == 0 && state == images.size()) {
				state = 0;
				isOver = 0;
			}
		}
	}
	else isOver = 1;
	counter.add();
}





//函数
void textureInit() {//初始化纹理，各种道具用的图片
	box_tex.loadFromFile("images/box.png");
	box_open_tex.loadFromFile("images/box_open.png");
	fheal_tex.loadFromFile("images/fheal.png");
	pheal_tex.loadFromFile("images/pheal.png");
	bomb_tex.loadFromFile("images/bomb.png");
	lvup_tex.loadFromFile("images/lvup.png");
	pause_tex.loadFromFile("images/pause.png");
	proplist.loadFromFile("images/proplist.png");
}


void boxInit() {//随机生成1-3个宝箱
	std::mt19937 rng;
	rng.seed(std::random_device()());//初始化随机种子
	std::uniform_int_distribution<std::mt19937::result_type> num_range(1, 3);
	int num = num_range(rng);//随机生成宝箱数
	int flag = num;

	while (flag > 0) {
		std::mt19937 rng;
		rng.seed(std::random_device()());//初始化随机种子
		std::uniform_int_distribution<std::mt19937::result_type> x_(300, 1100);//范围可修改
		std::uniform_int_distribution<std::mt19937::result_type> y_(300, 650);//范围可修改
		int temp_x = x_(rng);
		int temp_y = y_(rng);//初始化随机位置
		int temp_flag = 1;

		for (int i = 0; i < barrier.size(); i++) {//检测宝箱坐标是否和障碍物重合
			if (temp_x == barrier[i].x && temp_y == barrier[i].y) {
				temp_flag = 0;
				break;
			}
		}

		for (boxes_index = boxes.begin(); boxes_index != boxes.end(); boxes_index++) {//检测生成的宝箱是否重合
			if (boxes_index->getx() == temp_x && boxes_index->gety() == temp_y) {
				temp_flag = 0;
				break;
			}
		}

		if (temp_flag) {
			boxes.push_back(Box(temp_x, temp_y));
			flag--;
		}
	}
}


void boxShow() { //在地图上画宝箱
	for (boxes_index = boxes.begin(); boxes_index != boxes.end(); boxes_index++) {
		if (boxes_index->getState() == 1)//未打卡的宝箱
			window.draw(boxes_index->getCimage());
		else if (boxes_index->getState() == 0)//已打开的宝箱
			window.draw(boxes_index->getOimage());
	}
}


void propShow() { //显示掉落道具
	for (prop_index = prop.begin(); prop_index != prop.end(); prop_index++)
		window.draw(prop_index->getImage());
}


void propMenuShow() { //展示物品栏
	sf::Sprite proplist_{ proplist };
	proplist_.setScale(0.3f, 0.3f);
	proplist_.setPosition(630, 800);
	window.draw(proplist_);//画物品栏
}


void propPick() { //检测道具是否被拾取，若拾取则propmenu对应道具数目加1
	for (prop_index = prop.begin(); prop_index != prop.end(); prop_index++) {
		if (checkCollision(prop_index->getImage(), s4)) {//检测主角和道具碰撞
			switch (prop_index->getMode()) {//道具数目增加
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
			prop.erase(prop_index);//从掉落道具列表移除
		}
	}
}


void prop_to_master() {//使用对主角生效的道具
	int health = h1.getHp();//主角生命值
	int wplv = b1.getDamage();//武器等级
	if (propmenu[1].isChosen) {
		if (propmenu[1].num) {
			h1.setHp(5);//大血瓶，回满
			propmenu[1].num--;
			propmenu[1].reset();
		}
		else propmenu[1].reset();
	}
	else if (propmenu[2].isChosen) {//小血瓶回2
		if (propmenu[2].num) {
			if (health <= 5 - 2)h1.setHp(health + 2);
			else h1.setHp(5);
			propmenu[2].num--;
			propmenu[2].reset();
		}
		else propmenu[2].reset();
	}
	else if (propmenu[4].isChosen) {//武器升一级
		if (propmenu[4].num) {
			if (wplv < 3)b1.setDamage(wplv + 1);
			else b1.setDamage(3);
			propmenu[4].num--;
			propmenu[4].reset();
		}
		else propmenu[4].reset();
	}
}

// ---7.2 以下三个函数全部修改
void prop_bomb(sf::Vector2f mouse_xy) {//炸弹的使用，需要放到鼠标分支里
	sf::Texture bomb1, bomb2;
	bomb1.loadFromFile("images/bomb1");
	bomb2.loadFromFile("images/bomb2");
	if (propmenu[3].isChosen && propmenu[3].num) {
		effects.push_back(Effect{ 300 ,100, mouse_xy.x, mouse_xy.y });
		effects.back().addImage(bomb1);
		effects.back().addImage(bomb2);
		bombs.push(Bomb{ 150, mouse_xy.x, mouse_xy.y });
		propmenu[3].num--;
		propmenu[3].reset();
	}
	else if (propmenu[3].isChosen && !propmenu[3].num)propmenu[3].reset();
}


void prop_bomb_over() {//炸弹引信结束，要炸了 ---7.1
	sf::Texture burst1, burst2, burst3;
	burst1.loadFromFile("images/burst1");
	burst2.loadFromFile("images/burst2");
	burst3.loadFromFile("images/burst3");
	if (bombs.back().getOver()) {//炸弹引信到时间
		for (int i = 0; i < enemy.size(); i++) {//炸弹，对范围内的小怪有伤害
			int dx = enemySprite[i].getPosition().x - bombs.back().getx();
			int dy = enemySprite[i].getPosition().y - bombs.back().gety();
			if (dx * dx + dy * dy <= 50)enemy[i].beHitted(100);//炸弹伤害
		}
		effects.push_back(Effect{ 150,50,bombs.back().getx(),bombs.back().gety() });
		effects.back().addImage(burst1);
		effects.back().addImage(burst2);
		effects.back().addImage(burst3);
		bombs.pop();
	}
}


void prop_pause() {//时间停止生效和解除
	if (propmenu[5].isChosen && propmenu[5].num) {//时间停止，所有小怪停止移动一段时间
		pauses.push_back(Counter{ 400 });//时间停止400帧，时间4s
		for (int i = 0; i < enemy.size(); i++) {
			enemy[i].setSpeed(0);
		}
		propmenu[5].num--;
		propmenu[5].reset();
	}
	else if (propmenu[5].isChosen && !propmenu[5].num)propmenu[5].reset();

	if (pauses.back().getOver()) {//时间停止结束
		for (int i = 0; i < enemy.size(); i++)
			enemy[i].setSpeed(2);
		pauses.pop_back();
	}
}


void effectShow() {//道具特效
	for (effects_index = effects.begin(); effects_index != effects.end(); effects_index++) {
		if (!effects_index->getOver())effects_index->run();
		else {
			effects.erase(effects_index);
			effects_index--;
		}
	}
}





//函数封装  可封装到类中，如果需要
void prop_system_init() {//道具系统初始化
	textureInit();//初始化所有图片纹理
	boxInit();//初始化宝箱位置
}

//prop_bomb(Vector2f mouse_xy);//炸弹的使用，放到鼠标分支里

void prop_system_run_data() {//道具系统运行：修改数据
	propPick();//检测道具是否被拾取
	prop_to_master();//使用对主角生效的道具
	prop_bomb_over();//炸弹引信结束，要炸了 ---7.1
	prop_pause();//时间停止的使用和解除
}

void prop_system_run_draw() {//道具系统运行：绘制
	boxShow();//画宝箱
	propShow(); //显示掉落道具
	propMenuShow();//展示物品栏
	effectShow();//特效绘制
}
