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

// ��ײ��⺯��
bool checkCollision(Sprite s1, Sprite s2);
//{
	//return s1.getLocalBounds().intersects(s2.getLocalBounds());
//}


//��������
class Counter {//֡��������
	static int num;
	int no_;//���������
	int now{ 0 };//��ǰΪ����֡
	bool isOver{ 0 };//�Ƿ�ﵽ����
	bool isRunning{ 0 };//�Ƿ��ڼ���
	int limit{ 10 };//����
public:
	Counter();
	Counter(int lim);
	int getNow();
	int getLimit();
	int getNo_();
	bool getOver();
	bool getRun();
	void add();//������1
	void reset();//��������0
	void shutdown();//�رռ�����
	void setup();//���ü�����
	void setLimit(int lim);//��������
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
void Counter::add() {//��ѭ������ĩβִ��
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
void Counter::setLimit(int lim) {//��������
	limit = lim;
}


//��
class Box {//����
	float x{ 0 };
	float y{ 0 };
	bool state{ 1 };//δ��Ϊ1
	sf::Sprite cimage;//δ��
	sf::Sprite oimage;//�Ѵ�
public:
	Box() = default;
	Box(float x_, float y_);
	float getx();
	float gety();
	sf::Sprite getCimage();
	sf::Sprite getOimage();
	void detect(float hx, float hy);
	bool getState();
	void open();//�򿪱���
	void drop();//���ߵ���
};


class Prop {//������
protected:
	float x{ 0 }; //����x
	float y{ 0 }; //����y
	int mode{2}; //�������� 1:fheal 2:pheal 3:bomb 4:lvup 5:pause
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


class PropMenu {//�������ṹ��
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


//����    *����δ�����Ҫ�޸�
void textureInit();//��ʼ������ͼƬ����
void boxInit();//��ʼ������λ��
void boxShow();//������
void propShow(); //��ʾ�������
void propMenuShow();//չʾ��Ʒ��*
void prop_to_master();//ʹ�ö�������Ч�ĵ���
void prop_bomb(Vector2f mouse_xy);//ը����ʹ�ã���Ҫ�ŵ�����֧��
void prop_bomb_over();//ը�����Ž�����Ҫը�� ---7.1
void prop_pause();//ʱ��ֹͣ��ʹ�úͽ��
void effectShow();//��Ч����


//������װ ���Է�װ���࣬�����Ҫ�Ļ�
void prop_system_init();//����ϵͳ��ʼ�����ŵ�ѭ����֮ǰ
void prop_system_run_draw();//����ϵͳ����:��ͼ
void prop_system_run_data();//����ϵͳ����:�޸�����



//��������
//����extern�ı�����Ҫ�ŵ�main.cpp
extern Hero h1;//���ǵĶ���
extern Weapon b1;//�����Ķ���
extern sf::Sprite s4;//���ǵ�sprite
extern std::vector<Enemy>enemy;//���˵Ķ�������
extern std::vector<sf::Sprite>enemySprite;//����ͼ��Ķ�������
extern std::vector<Vector2f> barrier; //�ϰ���
extern RenderWindow window;//����

extern std::list<Box> boxes; //δ����ı���,ͨ��ʱ��Ҫ���
extern std::list<Box>::iterator boxes_index;  //�����б������
extern std::list<Prop>prop; //δʰȡ�ĵ���
extern std::list<Prop>::iterator prop_index; //�����б������
extern PropMenu propmenu[6]; //������ ������� ��Ҫpropmenu[0]��
extern std::vector<Counter>pauses;//ʱ��ֹͣ����ʱ
extern std::queue<Bomb>bombs;//ը������ʱ����
extern std::vector<Effect>effects;//��Ч����
extern std::vector<Effect>::iterator effects_index;

extern sf::Texture box_tex; //��������
extern sf::Texture box_open_tex; //�򿪵ı�������
extern sf::Texture fheal_tex;
extern sf::Texture pheal_tex;
extern sf::Texture bomb_tex;
extern sf::Texture lvup_tex;
extern sf::Texture pause_tex;
extern sf::Texture proplist;//����������




//���Ա����
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
void Box::drop() {//�������
	std::mt19937 rng;
	rng.seed(std::random_device()());//��ʼ���������
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
void Box::open() {//���䱻��
	/* ����������Ҫ�ӵ�main������֧��
	case sk::F://��F������
		for (boxes_index = boxes.begin(); boxes_index != boxes.end(); boxes_index++) {
			boxes_index->detect(h1.getCoord().x, h1.getCoord().y);
		}
		skills = 'F';
		break;
	*/
	state = 0;
}
void Box::detect(float hx, float hy) {//getter//����������
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

void PropMenu::reset() {//����ѡ��״̬
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





//����
void textureInit() {//��ʼ���������ֵ����õ�ͼƬ
	box_tex.loadFromFile("images/box.png");
	box_open_tex.loadFromFile("images/box_open.png");
	fheal_tex.loadFromFile("images/fheal.png");
	pheal_tex.loadFromFile("images/pheal.png");
	bomb_tex.loadFromFile("images/bomb.png");
	lvup_tex.loadFromFile("images/lvup.png");
	pause_tex.loadFromFile("images/pause.png");
	proplist.loadFromFile("images/proplist.png");
}


void boxInit() {//�������1-3������
	std::mt19937 rng;
	rng.seed(std::random_device()());//��ʼ���������
	std::uniform_int_distribution<std::mt19937::result_type> num_range(1, 3);
	int num = num_range(rng);//������ɱ�����
	int flag = num;

	while (flag > 0) {
		std::mt19937 rng;
		rng.seed(std::random_device()());//��ʼ���������
		std::uniform_int_distribution<std::mt19937::result_type> x_(300, 1100);//��Χ���޸�
		std::uniform_int_distribution<std::mt19937::result_type> y_(300, 650);//��Χ���޸�
		int temp_x = x_(rng);
		int temp_y = y_(rng);//��ʼ�����λ��
		int temp_flag = 1;

		for (int i = 0; i < barrier.size(); i++) {//��ⱦ�������Ƿ���ϰ����غ�
			if (temp_x == barrier[i].x && temp_y == barrier[i].y) {
				temp_flag = 0;
				break;
			}
		}

		for (boxes_index = boxes.begin(); boxes_index != boxes.end(); boxes_index++) {//������ɵı����Ƿ��غ�
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


void boxShow() { //�ڵ�ͼ�ϻ�����
	for (boxes_index = boxes.begin(); boxes_index != boxes.end(); boxes_index++) {
		if (boxes_index->getState() == 1)//δ�򿨵ı���
			window.draw(boxes_index->getCimage());
		else if (boxes_index->getState() == 0)//�Ѵ򿪵ı���
			window.draw(boxes_index->getOimage());
	}
}


void propShow() { //��ʾ�������
	for (prop_index = prop.begin(); prop_index != prop.end(); prop_index++)
		window.draw(prop_index->getImage());
}


void propMenuShow() { //չʾ��Ʒ��
	sf::Sprite proplist_{ proplist };
	proplist_.setScale(0.3f, 0.3f);
	proplist_.setPosition(630, 800);
	window.draw(proplist_);//����Ʒ��
}


void propPick() { //�������Ƿ�ʰȡ����ʰȡ��propmenu��Ӧ������Ŀ��1
	for (prop_index = prop.begin(); prop_index != prop.end(); prop_index++) {
		if (checkCollision(prop_index->getImage(), s4)) {//������Ǻ͵�����ײ
			switch (prop_index->getMode()) {//������Ŀ����
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
			prop.erase(prop_index);//�ӵ�������б��Ƴ�
		}
	}
}


void prop_to_master() {//ʹ�ö�������Ч�ĵ���
	int health = h1.getHp();//��������ֵ
	int wplv = b1.getDamage();//�����ȼ�
	if (propmenu[1].isChosen) {
		if (propmenu[1].num) {
			h1.setHp(5);//��Ѫƿ������
			propmenu[1].num--;
			propmenu[1].reset();
		}
		else propmenu[1].reset();
	}
	else if (propmenu[2].isChosen) {//СѪƿ��2
		if (propmenu[2].num) {
			if (health <= 5 - 2)h1.setHp(health + 2);
			else h1.setHp(5);
			propmenu[2].num--;
			propmenu[2].reset();
		}
		else propmenu[2].reset();
	}
	else if (propmenu[4].isChosen) {//������һ��
		if (propmenu[4].num) {
			if (wplv < 3)b1.setDamage(wplv + 1);
			else b1.setDamage(3);
			propmenu[4].num--;
			propmenu[4].reset();
		}
		else propmenu[4].reset();
	}
}

// ---7.2 ������������ȫ���޸�
void prop_bomb(sf::Vector2f mouse_xy) {//ը����ʹ�ã���Ҫ�ŵ�����֧��
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


void prop_bomb_over() {//ը�����Ž�����Ҫը�� ---7.1
	sf::Texture burst1, burst2, burst3;
	burst1.loadFromFile("images/burst1");
	burst2.loadFromFile("images/burst2");
	burst3.loadFromFile("images/burst3");
	if (bombs.back().getOver()) {//ը�����ŵ�ʱ��
		for (int i = 0; i < enemy.size(); i++) {//ը�����Է�Χ�ڵ�С�����˺�
			int dx = enemySprite[i].getPosition().x - bombs.back().getx();
			int dy = enemySprite[i].getPosition().y - bombs.back().gety();
			if (dx * dx + dy * dy <= 50)enemy[i].beHitted(100);//ը���˺�
		}
		effects.push_back(Effect{ 150,50,bombs.back().getx(),bombs.back().gety() });
		effects.back().addImage(burst1);
		effects.back().addImage(burst2);
		effects.back().addImage(burst3);
		bombs.pop();
	}
}


void prop_pause() {//ʱ��ֹͣ��Ч�ͽ��
	if (propmenu[5].isChosen && propmenu[5].num) {//ʱ��ֹͣ������С��ֹͣ�ƶ�һ��ʱ��
		pauses.push_back(Counter{ 400 });//ʱ��ֹͣ400֡��ʱ��4s
		for (int i = 0; i < enemy.size(); i++) {
			enemy[i].setSpeed(0);
		}
		propmenu[5].num--;
		propmenu[5].reset();
	}
	else if (propmenu[5].isChosen && !propmenu[5].num)propmenu[5].reset();

	if (pauses.back().getOver()) {//ʱ��ֹͣ����
		for (int i = 0; i < enemy.size(); i++)
			enemy[i].setSpeed(2);
		pauses.pop_back();
	}
}


void effectShow() {//������Ч
	for (effects_index = effects.begin(); effects_index != effects.end(); effects_index++) {
		if (!effects_index->getOver())effects_index->run();
		else {
			effects.erase(effects_index);
			effects_index--;
		}
	}
}





//������װ  �ɷ�װ�����У������Ҫ
void prop_system_init() {//����ϵͳ��ʼ��
	textureInit();//��ʼ������ͼƬ����
	boxInit();//��ʼ������λ��
}

//prop_bomb(Vector2f mouse_xy);//ը����ʹ�ã��ŵ�����֧��

void prop_system_run_data() {//����ϵͳ���У��޸�����
	propPick();//�������Ƿ�ʰȡ
	prop_to_master();//ʹ�ö�������Ч�ĵ���
	prop_bomb_over();//ը�����Ž�����Ҫը�� ---7.1
	prop_pause();//ʱ��ֹͣ��ʹ�úͽ��
}

void prop_system_run_draw() {//����ϵͳ���У�����
	boxShow();//������
	propShow(); //��ʾ�������
	propMenuShow();//չʾ��Ʒ��
	effectShow();//��Ч����
}
