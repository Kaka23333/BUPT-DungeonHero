
#if 1
//#pragma comment(lib,"sfml-graphics-d.lib")
#include <SFML/Graphics.hpp>  
#include<iostream>
#include <ctime>
#include <iostream>
#include <deque>
//齐伟哥的子弹
//我做了一些修改
//std::deque<Bullet> bullets{} 放进了类里
class Bullet {
public:
	int x;
	int y;
	sf::Vector2f direction{ 0, 0 };
	int coord_x;
	int coord_y;//射出点坐标
};
class Weapon {
public:
	int shot_speed{ 10 };        //子弹飞行速度
	int damage{ 1 };            //子弹伤害
	double bullet_delay{ 0.01 };  //子弹每发间隔
	int range{ 400 };           //子弹射程
	int x, y;
	std::deque<Bullet> bullets{};  //子弹XX武器专属
	sf::Texture image;//图片
	sf::Sprite bullet;//子弹精灵

public:
	Weapon(int x_, int y_, int damage_);
	void SetXY(int x_, int y_);//接口
	void check();//射程控制;
};
Weapon::Weapon(int x_, int y_, int damage_) :x{ x_ }, y{ y_ }, damage{ damage_ }{
	switch (damage) {
	case 1: image.loadFromFile("images/bullet1.png");
		bullet.setTexture(image); break;
	case 2: image.loadFromFile("images/bullet2.png");
		bullet.setTexture(image); break;
	case 3: image.loadFromFile("images/bullet3.png");
		bullet.setTexture(image); break;
	}
}
void Weapon::SetXY(int x_, int y_) {
	x = x_;
	y = y_;
}
//检测子弹的状况
void Weapon::check() {
	for (int i = bullets.size() - 1; i > -1 &&(pow(bullets[i].x - bullets[i].coord_x, 2) +
		pow(bullets[i].y - bullets[i].coord_y, 2) >
		range* range); i--) {
		bullets.pop_front();  //超出射程
		  // else if (bullets[i].coord_x) 撞到地形
		  // else if (bullets[i].coord_x) 撞到怪
	}
}
//英雄
#define DLX 80
const int size = 16;
const int windowWidth = 60;
const int windowHeight = 40;


class Hero {
public:
	int x, y;
	bool isLive = true;
	bool fire = false;
	short int dlx = DLX;
	short int HeroSpeed = 4;//每帧位移//速度
	short int RunType = 0;//跑动特效
	int HreoDirection = 3;//方向
	sf::Texture image;//图像
	Weapon wand{ x,y ,1 };//武器 magic stick hhh 魔杖 ,起始位置和伤害
	Hero(int x_, int y_) : x(x_), y(y_) {}
	void launch(){//启动
		std::cout << "开始!";
		HreoDirection = 3;//方向为右
	}
	
};
void renderBorder(sf::RenderWindow& window) {//碰撞检测
}
Hero Master(1, 1);

class {};
void update() {//移动
	//法师移动
	if (2== Master.HreoDirection ) Master.x -= Master.HeroSpeed;
	if (1== Master.HreoDirection ) Master.y += Master.HeroSpeed;
	if (3== Master.HreoDirection ) Master.x += Master.HeroSpeed;
	if (0== Master.HreoDirection ) Master.y -= Master.HeroSpeed;
	//if (Master.x >= windowWidth - 1)Master.isLive = false;
	//if (Master.x < 1) Master.isLive = false;
	//if (Master.y >= windowHeight - 1) Master.isLive = false;
	//if (Master.y < 1) Master.isLive = false;
	//边缘检查

	//子弹射击
	for (int i = 0; Master.wand.bullets.size() > i; i++) {
		Master.wand.bullets[i].coord_x += Master.wand.bullets[i].direction.x;
		Master.wand.bullets[i].coord_y += Master.wand.bullets[i].direction.y;
	}
		//switch (Master.wand.bullets[i].direction) {//子弹向量坐标变化
		//case 0:  Master.wand.bullets[i].coord_y -= Master.wand.shot_speed; break;
	//	case 1:  Master.wand.bullets[i].coord_y += Master.wand.shot_speed; break;
	//	case 2:  Master.wand.bullets[i].coord_x -= Master.wand.shot_speed; break;
	//	case 3:  Master.wand.bullets[i].coord_x += Master.wand.shot_speed; break;
	//
	//	}
	//}
}
void renderHero(sf::RenderWindow& window) {//调图作画
	if (2 == Master.HreoDirection || 0 == Master.HreoDirection)//0上1下2左3右{
	{
		if (++Master.RunType % 2)
			Master.image.loadFromFile("images/mleft1.png");
		else
			Master.image.loadFromFile("images/mleft2.png");
	}
	else {
		if (++Master.RunType % 2)
			Master.image.loadFromFile("images/mright1.png");
		else
			Master.image.loadFromFile("images/mright2.png");// sf::IntRect(10, 10, 32, 32));
	}
	sf::Sprite s(Master.image);//TempSprite
	s.setScale(0.25, 0.25);
	s.setPosition(Master.x, Master.y);
	window.draw(s);
	//子弹和武器
	Master.wand.check();//超射程销毁
	for (int i = 0; Master.wand.bullets.size() > i; i++) {
		//子弹随伤害改变调图
		sf::Sprite s2(Master.wand.image);//ps:不能复用

		//s2.setTexture(t1);// bullet;
		s2.setPosition(static_cast<float>(Master.wand.bullets[i].coord_x),
			static_cast<float> (Master.wand.bullets[i].coord_y));
		window.draw(s2);
	}
}
void render(sf::RenderWindow& window) {//
	renderBorder(window);//图像边界
	renderHero(window);//图像移动
}
int main(int argc, char const* argv[])
{
	sf::RenderWindow window(sf::VideoMode(1440, 900), "Dumbledore's adventures");
	sf::Clock clock;
	float dtTime = 0;
	float delay = 0.01;//tmd
	//自定义帧率
	Master.launch();
	sf::Event event;
	while (window.isOpen()) {//A!S!d!w!

		dtTime += clock.restart().asSeconds();

		if (dtTime > Master.wand.bullet_delay) Master.fire = true;
		//产生新子弹
		typedef sf::Keyboard sk;//重要sf::Keyboard sk 简写
		while (window.pollEvent(event)) {//事件轮询
			//std::cout << Master.HreoDirection;
			if (event.type == sf::Event::Closed)
				window.close();
			if(sf::Event::KeyPressed){
				switch (event.key.code) {//上0下1左2右三 02 13
				case sk::Left: Master.HreoDirection = 2; Master.dlx = 0; break;//方向控制
				case sk::Down: Master.HreoDirection = 1; Master.dlx = DLX; break;
				case sk::Right: Master.HreoDirection = 3; Master.dlx = DLX; break;
				case sk::Up: Master.HreoDirection = 0; Master.dlx = 0; break;
				}
			}
			if (Master.fire) {
				if (event.type == sf::Event::MouseButtonPressed) {
					sf::Vector2f temp_xy{ sf::Mouse::getPosition(window) };
					float temp =
						pow(Master.wand.shot_speed * Master.wand.shot_speed /
						(pow(temp_xy.x - Master.x, 2) + pow(temp_xy.y - Master.y, 2)),
							0.5);
					Master.wand.bullets.push_back(Bullet{ Master.x+Master.dlx ,Master.y,
											 sf::Vector2f{(temp_xy.x - Master.x) * temp,
														  (temp_xy.y - Master.y) * temp},
											 Master.x + Master.dlx,Master.y });
					Master.fire = false;
				}
				
			}

		}
		if (dtTime > delay) {//重要的时间间隔刷新函数
			dtTime = 0;
			update();//逻辑移动
		}
		window.clear(sf::Color::Black);
		render(window);//图像移动
		window.display();
	}
	return 0;
}

#endif
